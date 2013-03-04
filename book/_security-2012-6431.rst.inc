.. note::

    A partire da Symfony 2.0.20/2.1.5, il tag ``render`` di Twig accetta url assoluti
    invece di un percorso logico di un controllore. Questo risolve una importante
    questione di sicurezza (`CVE-2012-6431`_) segnalata sul blog ufficiale. Se si ha un'applicazione
    che usa una vecchia versione di Symfony o usa ancora la vecchia sintassi del tag ``render``,
    si dovrebbe aggiornare quanto prima.

.. _`CVE-2012-6431`: http://symfony.com/blog/security-release-symfony-2-0-20-and-2-1-5-released
