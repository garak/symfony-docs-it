.. index::
   single: Request; Trusted Proxies

Trusting Proxies
================

Se ci si trova dietro un proxy, come un bilanciatore di carico, è possibile che
siano inviate alcune informazioni, con gli header speciali ``X-Forwarded-*``.
Per esempio, l'header HTTP ``Host`` di solito si uda per restituire
l'host richiesto. Ma quando ci si trova dietro a un proxy, il vero host potrebbe
trovarsi nell'header``X-Forwarded-Host``.

Poiché gli header HTTP possono essere falsificati, Symfony2 *non* si fida degli
header dei proxy. Se si è dietro a un proxy, si deve indicare manualmente che
tale proxy è fidato::

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();
    // fidarsi solo degli header dei proxy che vengono da questo indirizzo IP
    $request->setTrustedProxies(array(192.0.0.1));

Configurare i nomi degli header
-------------------------------

I seguenti proxy sono fidati per impostazione predefinita:

* ``X-Forwarded-For`` Usato in :method:`Symfony\\Component\\HttpFoundation\\Request::getClientIp`;
* ``X-Forwarded-Host`` Usato in :method:`Symfony\\Component\\HttpFoundation\\Request::getHost`;
* ``X-Forwarded-Port`` Usato in :method:`Symfony\\Component\\HttpFoundation\\Request::getPort`;
* ``X-Forwarded-Proto`` Usato in :method:`Symfony\\Component\\HttpFoundation\\Request::getScheme` and :method:`Symfony\\Component\\HttpFoundation\\Request::isSecure`;

Se un reverse proxy usa un nome diverso per uno di questi header, lo si può
configurare tramite :method:`Symfony\\Component\\HttpFoundation\\Request::setTrustedHeaderName`::

    $request->setTrustedHeaderName(Request::HEADER_CLIENT_IP, 'X-Proxy-For');
    $request->setTrustedHeaderName(Request::HEADER_CLIENT_HOST, 'X-Proxy-Host');
    $request->setTrustedHeaderName(Request::HEADER_CLIENT_PORT, 'X-Proxy-Port');
    $request->setTrustedHeaderName(Request::HEADER_CLIENT_PROTO, 'X-Proxy-Proto');

Diffidare di alcuni header
--------------------------

Per impostazione predefinita, se si indica un indirizzo IP di un proxy come fidato, tutti e quattro gli header
elencati sopra saranno fidati. Se occorre indicare come fidati alcuni di questi header ma
non altri, lo si può fare::

    // disabilita la fiducia nell'header ``X-Forwarded-Proto``, sarà usato l'header predefinito
    $request->setTrustedHeaderName(Request::HEADER_CLIENT_PROTO, '');
