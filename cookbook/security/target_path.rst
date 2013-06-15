.. index::
   single: Sicurezza; Percorso di rinvio

Come cambiare il comportamento del percorso di rinvio predefinito
=================================================================

Per impostazione predefinita, il componente della sicurezza mantiene l'informazione
sull'URI dell'ultima richiesta in una variabile di sessione chiamata ``_security.main.target_path`` (in cui ``main``
è il nome del firewall, definito in ``security.yml``). Dopo un login eseguito con successo,
l'utente viene rinviato a tale percorso, per aiutarlo a continuare dalla
sua ultima pagina vista.

In alcune occasioni, tale comportamento è inatteso. Per sempio, quando l'URI dell'ultima
richiesta era un POST HTTP su una rotta configurata per consentire solo il metodo POST,
l'utente viene rinviato a tale rotta per ottenere un errore 404.

Per ovviare a questo problema, basta estendere la classe ``ExceptionListener``
e sovrascrivere il metodo chiamato ``setTargetPath()``.

Sovrascrivere prima il parametro ``security.exception_listener.class`` nel file di
configurazione. Lo si può fare nel file di configurazione principale (in
`app/config`) o nel file di configurazione di un bundle:

.. configuration-block::

        .. code-block:: yaml

            # src/Acme/HelloBundle/Resources/config/services.yml
            parameters:
                # ...
                security.exception_listener.class: Acme\HelloBundle\Security\Firewall\ExceptionListener

        .. code-block:: xml

            <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
            <parameters>
                <!-- ... -->
                <parameter key="security.exception_listener.class">Acme\HelloBundle\Security\Firewall\ExceptionListener</parameter>
            </parameters>

        .. code-block:: php

            // src/Acme/HelloBundle/Resources/config/services.php
            // ...
            $container->setParameter('security.exception_listener.class', 'Acme\HelloBundle\Security\Firewall\ExceptionListener');

Creare quindi il proprio ``ExceptionListener``::

    // src/Acme/HelloBundle/Security/Firewall/ExceptionListener.php
    namespace Acme\HelloBundle\Security\Firewall;

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\Security\Http\Firewall\ExceptionListener as BaseExceptionListener;

    class ExceptionListener extends BaseExceptionListener
    {
        protected function setTargetPath(Request $request)
        {
            // Non salavre il percorso per richieste XHR o diverse da GET
            // Si può aggiungere altra logica a piacere
            if ($request->isXmlHttpRequest() || 'GET' !== $request->getMethod()) {
                return;
            }

            parent::setTargetPath($request);
        }
    }

Aggiungere pure altra logica, se necessaria!
