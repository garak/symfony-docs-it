.. index::
   single: Service Container; Tags

Come far si che i servizi usino le etichette
============================================

Molti dei servizi centrali di Symfony2 dipendono da etichette per capire quali servizi
dovrebbero essere caricati, ricevere notifiche di eventi o per essere maneggiati in determinati modi.
Ad esempio, Twig usa l'etichetta ``twig.extension`` per caricare ulteriori estensioni.

Ma è possibile usare etichette anche nei propri bundle. Ad esempio nel caso in cui
uno dei propri servizi gestisca una collezione di un qualche genere o implementi una "lista" nella
quale diverse strategie alternative vengono provate fino a che una non risulti efficace. In questo articolo si userà
come esempio una "lista di trasporto" che è una collezione di classi che implementano ``\Swift_Transport``.
Usando questa lista il mailer di Swift proverà diversi tipi di trasporto fino a che uno non abbia successo.
Questo articolo si focalizza fondamentalmente sull'argomento della dependency injection.

Per iniziare si definisce la classe della ``ListaDiTrasporto``::

    namespace Acme\MailerBundle;
    
    class ListaDiTrasporto
    {
        private $trasporti;
    
        public function __construct()
        {
            $this->trasporti = array();
        }
    
        public function aggiungiTrasporto(\Swift_Transport  $trasporto)
        {
            $this->trasporti[] = $trasporto;
        }
    }

Quindi si definisce la lista come servizio:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/MailerBundle/Resources/config/services.yml
        parameters:
            acme_mailer.lista_trasporto.class: Acme\MailerBundle\ListaDiTrasporto
        
        services:
            acme_mailer.lista_trasporto:
                class: %acme_mailer.lista_trasporto.class%

    .. code-block:: xml

        <!-- src/Acme/MailerBundle/Resources/config/services.xml -->

        <parameters>
            <parameter key="acme_mailer.lista_trasporto.class">Acme\MailerBundle\ListaDiTrasporto</parameter>
        </parameters>
    
        <services>
            <service id="acme_mailer.lista_trasporto" class="%acme_mailer.lista_trasporto.class%" />
        </services>
        
    .. code-block:: php
    
        // src/Acme/MailerBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        
        $container->setParameter('acme_mailer.lista_trasporto.class', 'Acme\MailerBundle\ListaDiTrasporto');
        
        $container->setDefinition('acme_mailer.lista_trasporto', new Definition('%acme_mailer.lista_trasporto.class%'));

Definire un servizio con etichette personalizzate
-------------------------------------------------

A questo punto si vuole che diverse classi di ``\Swift_Transport`` vengano
istanziate e automaticamente aggiunte alla lista, usando il metodo ``aggiungiTrasporto``.
Come esempio si possono aggiungere i seguenti trasporti come servizi:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/MailerBundle/Resources/config/services.yml
        services:
            acme_mailer.transport.smtp:
                class: \Swift_SmtpTransport
                arguments:
                    - %mailer_host%
                tags:
                    -  { name: acme_mailer.transport }
            acme_mailer.transport.sendmail:
                class: \Swift_SendmailTransport
                tags:
                    -  { name: acme_mailer.transport }
    
    .. code-block:: xml

        <!-- src/Acme/MailerBundle/Resources/config/services.xml -->
        <service id="acme_mailer.transport.smtp" class="\Swift_SmtpTransport">
            <argument>%mailer_host%</argument>
            <tag name="acme_mailer.transport" />
        </service>
    
        <service id="acme_mailer.transport.sendmail" class="\Swift_SendmailTransport">
            <tag name="acme_mailer.transport" />
        </service>
        
    .. code-block:: php
    
        // src/Acme/MailerBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        
        $definitionSmtp = new Definition('\Swift_SmtpTransport', array('%mailer_host%'));
        $definitionSmtp->addTag('acme_mailer.transport');
        $container->setDefinition('acme_mailer.transport.smtp', $definitionSmtp);
        
        $definitionSendmail = new Definition('\Swift_SendmailTransport');
        $definitionSendmail->addTag('acme_mailer.transport');
        $container->setDefinition('acme_mailer.transport.sendmail', $definitionSendmail);

Si noti l'etichetta "acme_mailer.transport". Si vuole che il bundle riconosca
questi trasporti e li aggiunga, autonomamente, alla lista. Per realizzare questo
meccanismo è necessario definire un metodo ``build()`` nella classe ``AcmeMailerBundle``::

    namespace Acme\MailerBundle;
    
    use Symfony\Component\HttpKernel\Bundle\Bundle;
    use Symfony\Component\DependencyInjection\ContainerBuilder;
    
    use Acme\MailerBundle\DependencyInjection\Compiler\TransportCompilerPass;
    
    class AcmeMailerBundle extends Bundle
    {
        public function build(ContainerBuilder $contenitore)
        {
            parent::build($contenitore);
    
            $contenitore->addCompilerPass(new TransportCompilerPass());
        }
    }

Creazione del ``CompilerPass``
------------------------------

Si può notare che il metodo fa riferimento alla non ancora esistente classe ``TransportCompilerPass``.
Questa classe dovrà fare in modo che tutti i servizi etichettat come ``acme_mailer.transport``
vengano aggiunti alla classe ``ListaDiTrasporto`` tramite una chiamata al metodo
``aggiungiTrasporto()``. La classe ``TransportCompilerPass`` sarà simile alla seguente::

    namespace Acme\MailerBundle\DependencyInjection\Compiler;
    
    use Symfony\Component\DependencyInjection\ContainerBuilder;
    use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
    use Symfony\Component\DependencyInjection\Reference;
    
    class TransportCompilerPass implements CompilerPassInterface
    {
        public function process(ContainerBuilder $contenitore)
        {
            if (false === $contenitore->hasDefinition('acme_mailer.lista_trasporto')) {
                return;
            }
    
            $definizione = $contenitore->getDefinition('acme_mailer.lista_trasporto');
    
            foreach ($contenitore->findTaggedServiceIds('acme_mailer.transport') as $id => $attributi) {
                $definizione->addMethodCall('aggiungiTrasporto', array(new Reference($id)));
            }
        }
    }

Il metodo ``process()`` controllo l'esistenza di un servizio ``acme_mailer.lista_trasporto``,
quindi cerca tra tutti i servizi etichettati ``acme_mailer.transport``. Aggiunge
alla definizione del servizio ``acme_mailer.lista_trasporto`` una chiamata a
``aggiungiTrasporto()`` per ogni servizio "acme_mailer.transport" trovato.
Il primo argomento di ognuna di queste chiamate sarà lo stesso servizio di trasporto
della posta.

.. note::
    
    Per convenzione, in nomi di etichette sono formati dal nome del bundle(minuscolo
    con il trattino basso come separatore), seguito dal punto e, infine, dal nome
    "reale": perciò, l'etichetta "transport" di AcmeMailerBundle sarà ``acme_mailer.transport``.

Definizione dei servizi compilati
---------------------------------

Aggiungere il passo della compilazione avrà come risultato la creazione, in automatico, 
delle seguenti righe di codice nella versione compilata del contenitore di servizi. Nel caso si
stia lavorando nell'ambiente "dev", si apra il file ``/cache/dev/appDevDebugProjectContainer.php``
e si cerchi il metodo ``getTransportChainService()``. Dovrebbe essere simile al seguente::

    protected function getAcmeMailer_ListaTrasportoService()
    {
        $this->services['acme_mailer.lista_trasporto'] = $instance = new \Acme\MailerBundle\ListaDiTrasporto();

        $instance->aggiungiTrasporto($this->get('acme_mailer.transport.smtp'));
        $instance->aggiungiTrasporto($this->get('acme_mailer.transport.sendmail'));

        return $instance;
    }
