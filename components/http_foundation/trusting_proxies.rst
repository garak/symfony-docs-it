.. index::
   single: Request; Trusted Proxies

Trusting Proxies
================

Se ci si trova dietro un proxy, come un bilanciatore di carico, è possibile che
siano inviate alcune informazioni, con gli header speciali ``X-Forwarded-*``.
Per esempio, l'header HTTP ``Host`` di solito si uda per restituire
l'host richiesto. Ma quando ci si trova dietro a un proxy, il vero host potrebbe
trovarsi nell'header``X-Forwarded-Host``.

(traduzione da completare...)
