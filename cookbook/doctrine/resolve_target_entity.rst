.. index::
   single: Doctrine; Risolvere entità bersaglio
   single: Doctrine; Definire relazioni con classi astratte e interfacce

Definire relazioni con classi astratte e interfacce
===================================================

Uno degli scopi dei bundle è quello di creare insiemi di funzionalità
che non abbiamo molte dipendenze o che non ne abbiano alcuna, consentendone l'uso
in altre applicazioni, senza includere cose non strettamente necessarie.

Doctrine 2.2 include una nuova utility chiamata ``ResolveTargetEntityListener``,
che funziona intercettando alcune chiamate in Doctrine e riscrivendo i parametri
``targetEntity`` dei propri metadati di mappatura a runtime. Ciò vuol dire che
si ha la possibilità nel proprio bundle di usare interfacce o classi astratte nelle
mappature e aspettarsi mappature corrette su entità concrete a runtime.

Questa funzionalità consente di definire relazioni tra entità diverse,
senza renderle strettamente dipendenti.

Scenario
--------

Si supponga di avere un `InvoiceBundle`, che fornisce funzionalità per fatturazioni,
e un `CustomerBundle`, che contiene strumenti di gestione dei clienti. Si vuole
mantenerli separati, perché possono essere usati in altri sistemi in cui uno dei due
non ci sia, ma si ha un'applicazione che deve usarli insieme.

In questo caso, si ha un entità ``Invoice``, con una relazione a uno oggetto non
esistente, un ``InvoiceSubjectInterface``. Lo scopo è fare in modo che
``ResolveTargetEntityListener`` sostituisca ogni menzione all'interfaccia con
un oggetto reale, che implementi tale interfaccia.

Preparazione
------------

Usiamo alcune entità basilari (incomplete, per brevità)
per spiegare come preparare e usare ``ResolveTargetEntityListener``.

Un'entità per il cliente::

    // src/Acme/AppBundle/Entity/Customer.php

    namespace Acme\AppBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Acme\CustomerBundle\Entity\Customer as BaseCustomer;
    use Acme\InvoiceBundle\Model\InvoiceSubjectInterface;

    /**
     * @ORM\Entity
     * @ORM\Table(name="customer")
     */
    class Customer extends BaseCustomer implements InvoiceSubjectInterface
    {
        // Nel nostro esempio, ogni metodo definito in InvoiceSubjectInterface
        // è già implementato in BaseCustomer
    }

Un'entità per la fattura::

    // src/Acme/InvoiceBundle/Entity/Invoice.php

    namespace Acme\InvoiceBundle\Entity;

    use Doctrine\ORM\Mapping AS ORM;
    use Acme\InvoiceBundle\Model\InvoiceSubjectInterface;

    /**
     * Rappresenta una fattura.
     *
     * @ORM\Entity
     * @ORM\Table(name="invoice")
     */
    class Invoice
    {
        /**
         * @ORM\ManyToOne(targetEntity="Acme\InvoiceBundle\Model\InvoiceSubjectInterface")
         * @var InvoiceSubjectInterface
         */
        protected $subject;
    }

Una InvoiceSubjectInterface::

    // src/Acme/InvoiceBundle/Model/InvoiceSubjectInterface.php

    namespace Acme\InvoiceBundle\Model;

    /**
     * Un'interfaccia che l'oggetto fattura dovrebbe implementare.
     * Nella maggior parte dei casi, solo un singolo oggetto dovrebbe implementare
     * questa interfaccia, perché ResolveTargetEntityListener può cambiare la
     * destinazione di un unico oggetto.
     */
    interface InvoiceSubjectInterface
    {
        // Elencare ogni ulteriore metodo a cui InvoiceBundle
        // avrà bisogno di accedere sull'oggetto, in modo tale da
        // assicurarsi di avere accesso a tali metodi.

        /**
         * @return string
         */
        public function getName();
    }

Occorre quindi configurare l'ascoltatore, che dice a DoctrineBundle
come eseguire la sostituzione:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine:
            # ...
            orm:
                # ...
                resolve_target_entities:
                    Acme\InvoiceBundle\Model\InvoiceSubjectInterface: Acme\AppBundle\Entity\Customer

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:doctrine="http://symfony.com/schema/dic/doctrine"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/doctrine http://symfony.com/schema/dic/doctrine/doctrine-1.0.xsd">

            <doctrine:config>
                <doctrine:orm>
                    <!-- ... -->
                    <doctrine:resolve-target-entity interface="Acme\InvoiceBundle\Model\InvoiceSubjectInterface">Acme\AppBundle\Entity\Customer</doctrine:resolve-target-entity>
                </doctrine:orm>
            </doctrine:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine', array(
            'orm' => array(
                // ...
                'resolve_target_entities' => array(
                    'Acme\InvoiceBundle\Model\InvoiceSubjectInterface' => 'Acme\AppBundle\Entity\Customer',
                ),
            ),
        ));

Considerazioni finali
---------------------

Con ``ResolveTargetEntityListener``, si è in grado di disaccoppiare i propri
bundle, rendendoli usabili da soli, ma ancora in grado di definire
relazioni tra oggetti diversi. Usando tali metodi,
i bundle saranno più facili da manutenere in modo indipendente.
