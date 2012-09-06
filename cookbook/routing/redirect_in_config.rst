.. index::
   single: Routing; Configurare un rinvio a un'altra rotta senza controllori ad hoc

Configurare un rinvio a un'altra rotta senza controllori ad hoc
===============================================================

Questa ricetta spiega come configurare un rinvio da una rotta a un'altra,
senza usare un controllore ad hoc.

Ipotizziamo che non ci sia un controllore predefinito per il percorso ``/``
dell'applicazione e che si vogliano rinviare le richieste ad ``/app``.

La configurazione sarà simile a questa:

.. code-block:: yaml

    AppBundle:
        resource: "@App/Controller/"
        type:     annotation
        prefix:   /app

    root:
        pattern: /
        defaults:
            _controller: FrameworkBundle:Redirect:urlRedirect
            path: /app
            permanent: true

Il bundle ``AppBundle`` deve essere registrato per gestire tutte le richieste ad ``/app``.

Configuriamo una rotta per il percorso ``/`` e lo facciamo gestire a :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\RedirectController`.
Questo controllore offre due metodi per rinviare le richieste:

* ``redirect`` rinvia a un'altra *rotta*. Occorre fornire il parametro ``route``
  con il *nome* della rotta a cui si vuole rinviare.

* ``urlRedirect`` rinvia a un altro *percorso*. Occorre fornire il parametro ``path``
  con il percorso della risorsa a cui si vuole rinviare.

The ``permanent`` switch tells both methods to issue a 301 HTTP status code.
