.. index::
   single: Sicurezza; Puntare il percorso di rinvio

Come cambiare il comportamento predefinito del puntamento del percorso
======================================================================

Per impostazione predefinita, il componente della sicurezza mantiene le informazioni
sull'URI dell'ultima richiesta in una variabile di sessione, chiamata ``_security.target_path``.
Dopo che un accesso viene eseguito, l'utente viene rinviato a questo percorso, come aiuto
per continuare dall'ultima pagina visitata.

In alcune occasioni, questo comportamento è inatteso. Per esempio, quando l'URI dell'ultima
richiesta era un POST HTTP su una rotta configurata per consentire solo il metodo POST,
l'utente rinviato a tale rotta otterrebbe solo un errore 404.

Per aggirare questo comportamento, occorre semplicemente estendere la classe
``ExceptionListener`` e sovrascrivere il metodo chiamato ``setTargetPath()``.

Come prima cosa, sovrascrivere il parametro ``security.exception_listener.class`` nel
proprio file di configurazione. Lo si può fare dalla propria configurazione principale
(in `app/config`) oppure in un file di configurazione importato da un bundle:

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

Quindi, crare il proprio ``ExceptionListener``::

    // src/Acme/HelloBundle/Security/Firewall/ExceptionListener.php
    namespace Acme\HelloBundle\Security\Firewall;

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\Security\Http\Firewall\ExceptionListener as BaseExceptionListener;

    class ExceptionListener extends BaseExceptionListener
    {
        protected function setTargetPath(Request $request)
        {
            // Non salvare il percorso del puntamento per richieste XHR o diverse da GET
            // Si può aggiungere altra logica, all'occorrenza
            if ($request->isXmlHttpRequest() || 'GET' !== $request->getMethod()) {
                return;
            }

            $request->getSession()->set('_security.target_path', $request->getUri());
        }
    }

Si può aggiungere tutta la logica necessaria ai propri scopi!