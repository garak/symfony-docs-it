.. index::
   single: Request; Proxy fidati

Proxy fidati
============

.. tip::

    Se si usa il framework Symfony, si inizi leggendo
    :doc:`/cookbook/request/load_balancer_reverse_proxy`.

Se ci si trova dietro un proxy, come un bilanciatore di carico, è possibile che
siano inviate alcune informazioni con gli header speciali ``X-Forwarded-*``.
Per esempio, l'header HTTP ``Host`` di solito si usa per restituire
l'host richiesto. Ma, quando ci si trova dietro a un proxy, il vero host potrebbe
trovarsi nell'header ``X-Forwarded-Host``.

Poiché gli header HTTP possono essere falsificati, Symfony2 *non* si fida degli
header dei proxy. Se si è dietro a un proxy, si deve indicare manualmente che
tale proxy è fidato.

.. versionadded:: 2.3
    È stato aggiunto il supporto alla notazione CIDR , quindi si possono inserire
    intere sottoreti (p.e. ``10.0.0.0/8``, ``fc00::/7``).

.. code-block:: php

    use Symfony\Component\HttpFoundation\Request;

    // fidarsi solo degli header dei proxy che vengono da questo indirizzo IP
    Request::setTrustedProxies(array('192.0.0.1', '10.0.0.0/8'));

.. note::

   Quando si usa il reverse proxy interno di Symfony (``AppCache.php``), assicurarsi di aggiungere
   ``127.0.0.1`` alla lista dei proxy fidati.


Configurare i nomi degli header
-------------------------------

I seguenti proxy sono fidati per impostazione predefinita:

* ``X-Forwarded-For`` Usato in :method:`Symfony\\Component\\HttpFoundation\\Request::getClientIp`;
* ``X-Forwarded-Host`` Usato in :method:`Symfony\\Component\\HttpFoundation\\Request::getHost`;
* ``X-Forwarded-Port`` Usato in :method:`Symfony\\Component\\HttpFoundation\\Request::getPort`;
* ``X-Forwarded-Proto`` Usato in :method:`Symfony\\Component\\HttpFoundation\\Request::getScheme` e :method:`Symfony\\Component\\HttpFoundation\\Request::isSecure`;

Se un reverse proxy usa un nome diverso per uno di questi header, lo si può
configurare tramite :method:`Symfony\\Component\\HttpFoundation\\Request::setTrustedHeaderName`::

    Request::setTrustedHeaderName(Request::HEADER_CLIENT_IP, 'X-Proxy-For');
    Request::setTrustedHeaderName(Request::HEADER_CLIENT_HOST, 'X-Proxy-Host');
    Request::setTrustedHeaderName(Request::HEADER_CLIENT_PORT, 'X-Proxy-Port');
    Request::setTrustedHeaderName(Request::HEADER_CLIENT_PROTO, 'X-Proxy-Proto');

Diffidare di alcuni header
--------------------------

Per impostazione predefinita, se si indica un indirizzo IP di un proxy come fidato, tutti e quattro gli header
elencati sopra saranno fidati. Se occorre indicare come fidati alcuni di questi header ma
non altri, lo si può fare::

    // disabilita la fiducia nell'header X-Forwarded-Proto, sarà usato l'header predefinito
    Request::setTrustedHeaderName(Request::HEADER_CLIENT_PROTO, '');
