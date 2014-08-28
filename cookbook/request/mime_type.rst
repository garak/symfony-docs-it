.. index::
   single: Richiesta; Aggiungere un formato di richiesta e un tipo mime

Registrare un nuovo formato di richiesta e un nuovo tipo mime
=============================================================

Ogni richiesta ha a un "formato" (come ``html``, ``json``), che viene usato
per determinare il tipo di contenuto che dovrà essere restituito nella risposta.
Il formato della richiesta, accessibile tramite
:method:`Symfony\\Component\\HttpFoundation\\Request::getRequestFormat`,
viene infatti utilizzato per definire il tipo MIME dell'intestazione ``Content-Type`` 
dell'oggetto ``Response``. Symfony contiene una mappa dei formati più comuni (come 
``html``, ``json``) e del corrispettivo tipo MIME (come ``text/html``,
``application/json``). È comunque possibile aggiungere nuovi formati/tipi MIME.
In questo documento si vedrà come aggiungere un nuovo formato ``jsonp``
e il corrispondente tipo MIME.

.. versionadded:: 2.5
    La possibilità di configurare i formati della richiesta è stata introdotta in Symfony 2.5.

Configurare un nuovo formato
----------------------------

FrameworkBundle registra un sottoscrittore, che aggiungerà i formati alle richieste in arrivo.

Tutto quello che occorre fare è configurare il formato ``jsonp``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            request:
                formats:
                    jsonp: 'application/javascript'

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:framework="http://symfony.com/schema/dic/symfony"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd
                http://symfony.com/schema/dic/symfony
                http://symfony.com/schema/dic/symfony/symfony-1.0.xsd"
        >
            <framework:config>
                <framework:request>
                    <framework:format name="jsonp">
                        <framework:mime-type>application/javascript</framework:mime-type>
                    </framework:format>
                </framework:request>
            </framework:config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'request' => array(
                'formats' => array(
                    'jsonp' => 'application/javascript',
                ),
            ),
        ));

.. tip::

    Si possno anche associare più tipi MIME allo stesso formato, ma si noti che
    il tipo preferito deve essere il primo e che sarà usato come content-type:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/config.yml
            framework:
                request:
                    formats:
                        csv: ['text/csv', 'text/plain']

        .. code-block:: xml

            <!-- app/config/config.xml -->
            <?xml version="1.0" encoding="UTF-8" ?>

            <container xmlns="http://symfony.com/schema/dic/services"
                xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xmlns:framework="http://symfony.com/schema/dic/symfony"
                xsi:schemaLocation="http://symfony.com/schema/dic/services
                    http://symfony.com/schema/dic/services/services-1.0.xsd
                    http://symfony.com/schema/dic/symfony
                    http://symfony.com/schema/dic/symfony/symfony-1.0.xsd"
            >
                <framework:config>
                    <framework:request>
                        <framework:format name="csv">
                            <framework:mime-type>text/csv</framework:mime-type>
                            <framework:mime-type>text/plain</framework:mime-type>
                        </framework:format>
                    </framework:request>
                </framework:config>
            </container>

        .. code-block:: php

            // app/config/config.php
            $container->loadFromExtension('framework', array(
                'request' => array(
                    'formats' => array(
                        'jsonp' => array(
                            'text/csv',
                            'text/plain',
                        ),
                    ),
                ),
            ));
