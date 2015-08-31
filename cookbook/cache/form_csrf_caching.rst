.. index::
    single: Cache; CSRF; Form

Cache di pagine con form protette con CSRF
==========================================

I token CSRF sono pensati per variare da utente a utente. Per questo motivo, occorre
cautela se si provano a mettere in cache pagine con form che li includano.

Per maggiori informazioni su come funziona la protezione CSRF in Symfony, fare
riferimento a :ref:`protezione CSRF <forms-csrf>`.

Perché una pagina in cache con CSRT è problematica
--------------------------------------------------

Di solito, a ciascun utente è assegnato un token CSRF univoco, memorizzato nella
sessione per la validazione. Questo vuol dire che se si mette in cache una pagina con
un form che contiene token CSRF, si metterà in cache il token CSRF solo per il *primo*
utente. Quando un utente compila il form, il token non corrisponderà più a quello
memorizzato in sessione e tutti gli utenti (tranne il primo) vedranno fallire la
validazione all'invio del form.

In effetti, molti reverse proxy (come  Varnish) rifiuteranno di mettere in cache una pagina
con un token CSRF. Questo perché viene inviato un cookie, per preservare
la sessione PHP aperta e il comportamento predefinito di Varnish è di non mettere in cache
richieste HTTP con cookie.

Come mettere in cache le pagine e avere ugualmente protezione CSRF
------------------------------------------------------------------

Per mettere in cache una pagina che contenga un token CSRF, si possono usare tecniche più avanzate
di cache, come i :ref:`frammenti ESI <edge-side-includes>`, in cui si mette in cache
l'intera pagina e si include il form in un tag ESI, senza alcuna cache.

Un'altra opzione è quella di caricare il form tramite una richiesta AJAX non in cache, ma
mettendo in cache il resto della risposta HTML.

Si può anche solo caricare il token CSRF con una richiesta AJAX e sostituire il valore
del campo.

.. _`Cross-site request forgery`: http://en.wikipedia.org/wiki/Cross-site_request_forgery
.. _`Security CSRF Component`: https://github.com/symfony/security-csrf