.. index::
    single: Routing; Rinvii di URL con barra finale

Rinvii di URL con barra finale
==============================

Lo scopo di questa ricetta è dimostrare come rinviare URL con una
barra finale allo stesso URL, ma senza barra finale
(per esempio da ``/en/blog/`` a ``/en/blog``).

Creare un controllore a cui corrisponda un qualsiasi URL con barra finale, togliere
la barra finale (mantenendo i parametri della query, se presenti) e rinviare al
nuovo URL con un codice di stato 301 come risposta::

    // src/Acme/DemoBundle/Controller/RedirectingController.php
    namespace Acme\DemoBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Request;

    class RedirectingController extends Controller
    {
        public function removeTrailingSlashAction(Request $request)
        {
            $pathInfo = $request->getPathInfo();
            $requestUri = $request->getRequestUri();

            $url = str_replace($pathInfo, rtrim($pathInfo, ' /'), $requestUri);

            return $this->redirect($url, 301);
        }
    }

Dopo di questo, creare una rotta che punti a tale controllore, che sia corrisposta da qualsiasi URL
con una barra finale. Assicurarsi in inserire questa rotta alla fine,
come spiegato sotto:

.. configuration-block::

    .. code-block:: yaml

        remove_trailing_slash:
            path: /{url}
            defaults: { _controller: AcmeDemoBundle:Redirecting:removeTrailingSlash }
            requirements:
                url: .*/$
                _method: GET

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing">
            <route id="remove_trailing_slash" path="/{url}">
                <default key="_controller">AcmeDemoBundle:Redirecting:removeTrailingSlash</default>
                <requirement key="url">.*/$</requirement>
                <requirement key="_method">GET</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add(
            'remove_trailing_slash',
            new Route(
                '/{url}',
                array(
                    '_controller' => 'AcmeDemoBundle:Redirecting:removeTrailingSlash',
                ),
                array(
                    'url' => '.*/$',
                    '_method' => 'GET',
                )
            )
        );

.. note::

    Il rinvio di una richiesta POST non funziona bene nei vecchi browser. Un 302
    su una richiessta POST manderebbe una richiesta GET dopo il rinvio per questioni
    di compatibilità. Per questo motivo, la rotta qui corrisponde solo a richieste GET.

.. caution::

    Assicurarsi di includere questa rotta nella configurazione delle rotte
    all'ultimo posto della lista. In caso contrario, si rischia di rinviare rotte
    reali (incluse quelle predefinite di Symfony2), che hanno effettivamente una barra
    finale nel percorso.
