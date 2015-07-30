Problemi di sicurezza
=====================

Questo documento spiega la gestione da parte della squadra di Symfony  dei problemi di sicurezza
di Symfony (in cui "Symfony" è il codice ospitato nel `repository Git`_
``symfony/symfony``).

Segnalare un problema di sicurezza
----------------------------------

Se si è trovato un problema di sicurezza in Symfony, non utilizzare la
lista o il bug tracker e non diffonderlo pubblicamente. Tutte le questioni di
sicurezza devono essere inviate a **security [at] symfony-project.com**. Le email
inviate a questo indirizzo verranno inoltrate alla squadra di sviluppo di Symfony .

Processo di risoluzione
-----------------------

Per ogni rapporto, prima si cercherà di confermare la vulnerabilità. Quando
confermata, la squadra di sviluppo lavorerà a una soluzione seguendo questi passi:

#. Inviare un riconoscimento al segnalatore;
#. Lavorare su una patch;
#. Ottenere un identificatore CVE da mitre.org;
#. Scrivere un annuncio sul `blog`_ di Symfony, che descriva la vulnerabilità.
   Tale post dovrebbe contenere le seguenti informazioni:

   * un titolo che includa sempre la stringa "Security release";
   * una descrizione della vulnerabilità;
   * le versioni afflitte;
   * i possibili exploit;
   * come applicare patch/aggiornamenti/workaround alle applicazioni afflitte;
   * l'identificatore CVE;
   * riconoscimenti.
#. Inviare patch e annuncio al segnalante per una revisione;
#. Applicare la patch a tutte le versioni di Symfony in manutenzione;
#. Pacchettizzare nuove versioni per tutte le versioni afflitte;
#. Pubblicare il post sul `blog`_ ufficiale di Symfony (va anche aggiunti alla
   categoria "`Security Advisories`_");
#. Aggiornare la lista degli avvisi di sicurezza (vedere sotto).

.. note::

    I rilasci che includono questioni di sicurezza non andrebbero fatti di sabato o
    domenica, a meno che la vulnerabilità non sia stata resa pubblica.

.. note::

   Mentre la patch è in corso di lavorazione, si prega di non rivelare pubblicamente la problematica.

.. note::

    La risoluzione può prendere tra un paio di giorni a un mese, a seconda
    della complessità e del coordinamento tra i progetti a valle (vedere
    il paragrafo successivo).

Collaborazione con progetti open source a valle
-----------------------------------------------

Poiché Symfony è usato da molti progetti open source, il modo in cui la
squadra di sicurezza di Symfony collabora sulle problematiche di sicurezza è stata standardizzata
con i progetti a valle. Il progetto funziona come segue:

#. Dopo che la squadra di sicurezza di Symfony ha riconosciuto la problematica, invia
   immediatamente una email alle squadre di sicurezza dei progetti a valle, per informarli
   della problematica;

#. La squadra di sicurezza di Symfony crea un repository Git privato, per facilitare la
   collaborazione sulla problematica. L'accesso a tale repository è fornito all
   squadra di sicurezza di Symfony, ai contributori du Symfony che hanno avuto impatto sulla
   problematica e a un rappresentante i ogni progetto a valle;

#. Le persone che accedono al repository privato lavorano a una soluzione per
   risolvere la problematica, tramite richieste di pull, revisioni di codice e commenti;

#. Una volta trovata la soluzione, tutti i progetti coinvolti collaborano per trovare
   la data migliore per un rilascio congiunto (non c'è garanzia che tutti i rilasci saranno
   contemporanei, ma si tenterà il più possibili di pubblicarli nello stesso periodo). Quando
   non si ritiene che la problematica abbia subito degli exploit, un periodo di due settimane
   sembra essere ragionevole.

La lista dei progetti a valle partecipanti a tale processo è mantenuta più corta
possibile, per meglio gestire il flusso di informazioni riservate, prima
della pubblicazione. Per questo motivo, i progetti saranno inclusi a sola discrezione
della squadra di sicurezza di Symfony.

A oggi, i seguenti progetti hanno approvato questo processo e sono parte dei
progetti a valle inclusi:

* Drupal (solitamente con rilasci di venerdì)
* eZPublish

Bollettini di sicurezza
-----------------------

Questa sezione elenca le vulnerabilità di sicurezza che sono state risolte in Symfony,
partendo da Symfony 1.0.0:

* 1 aprile 2015: `CVE-2015-2309: Unsafe methods in the Request class <http://symfony.com/blog/cve-2015-2309-unsafe-methods-in-the-request-class>`_ (Symfony 2.3.27, 2.5.11 e 2.6.6)
* 1 aprile 2015: `CVE-2015-2308: Esi Code Injection <http://symfony.com/blog/cve-2015-2308-esi-code-injection>`_ (Symfony 2.3.27, 2.5.11 e 2.6.6)
* 3 settembre 2014: `CVE-2014-6072: CSRF vulnerability in the Web Profiler <http://symfony.com/blog/cve-2014-6072-csrf-vulnerability-in-the-web-profiler>`_ (Symfony 2.3.19, 2.4.9 e 2.5.4)
* 3 settembre 2014: `CVE-2014-6061: Security issue when parsing the Authorization header <http://symfony.com/blog/cve-2014-6061-security-issue-when-parsing-the-authorization-header>`_ (Symfony 2.3.19, 2.4.9 e 2.5.4)
* 3 settembre 2014: `CVE-2014-5245: Direct access of ESI URLs behind a trusted proxy <http://symfony.com/blog/cve-2014-5245-direct-access-of-esi-urls-behind-a-trusted-proxy>`_ (Symfony 2.3.19, 2.4.9 e 2.5.4)
* 3 settembre 2014: `CVE-2014-5244: Denial of service with a malicious HTTP Host header <http://symfony.com/blog/cve-2014-5244-denial-of-service-with-a-malicious-http-host-header>`_ (Symfony 2.3.19, 2.4.9 e 2.5.4)
* 15 luglio 2014: `Security releases: Symfony 2.3.18, 2.4.8, and 2.5.2 released <http://symfony.com/blog/security-releases-cve-2014-4931-symfony-2-3-18-2-4-8-and-2-5-2-released>`_ (`CVE-2014-4931 <http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2014-4931>`_)
* 10 ottobre 2013: `Security releases: Symfony 2.0.25, 2.1.13, 2.2.9, and 2.3.6 released <http://symfony.com/blog/security-releases-cve-2013-5958-symfony-2-0-25-2-1-13-2-2-9-and-2-3-6-released>`_ (`CVE-2013-5958 <http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2013-5958>`_)
* 7 agosto 2013: `Security releases: Symfony 2.0.24, 2.1.12, 2.2.5, and 2.3.3 released <http://symfony.com/blog/security-releases-symfony-2-0-24-2-1-12-2-2-5-and-2-3-3-released>`_ (`CVE-2013-4751 <http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2013-4751>`_ e `CVE-2013-4752 <http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2013-4752>`_)
* 17 gennaio 2013: `Security release: Symfony 2.0.22 and 2.1.7 released <http://symfony.com/blog/security-release-symfony-2-0-22-and-2-1-7-released>`_ (`CVE-2013-1348 <http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2013-1348>`_ e `CVE-2013-1397 <http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2013-1397>`_)
* 20 dicembre 2012: `Security release: Symfony 2.0.20 and 2.1.5 <http://symfony.com/blog/security-release-symfony-2-0-20-and-2-1-5-released>`_  (`CVE-2012-6431 <http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2012-6431>`_ e `CVE-2012-6432 <http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2012-6432>`_)
* 29 novembre 2012: `Security release: Symfony 2.0.19 and 2.1.4 <http://symfony.com/blog/security-release-symfony-2-0-19-and-2-1-4>`_
* 25 novembre 2012: `Security release: symfony 1.4.20 released  <http://symfony.com/blog/security-release-symfony-1-4-20-released>`_
* 28 agosto 2012: `Security Release: Symfony 2.0.17 released <http://symfony.com/blog/security-release-symfony-2-0-17-released>`_
* 30 maggio 2012: `Security Release: symfony 1.4.18 released <http://symfony.com/blog/security-release-symfony-1-4-18-released>`_
* 24 febbraio 2012: `Security Release: Symfony 2.0.11 released <http://symfony.com/blog/security-release-symfony-2-0-11-released>`_
* 16 novembre 2011: `Security Release: Symfony 2.0.6 <http://symfony.com/blog/security-release-symfony-2-0-6>`_
* 21 marzo 2011: `symfony 1.3.10 and 1.4.10: security releases <http://symfony.com/blog/symfony-1-3-10-and-1-4-10-security-releases>`_
* 29i giugno 2010: `Security Release: symfony 1.3.6 and 1.4.6 <http://symfony.com/blog/security-release-symfony-1-3-6-and-1-4-6>`_
* 31 maggio 2010: `symfony 1.3.5 and 1.4.5 <http://symfony.com/blog/symfony-1-3-5-and-1-4-5>`_
* 25 febbraio 2010: `Security Release: 1.2.12, 1.3.3 and 1.4.3 <http://symfony.com/blog/security-release-1-2-12-1-3-3-and-1-4-3>`_
* 13 febbraio, 2010: `symfony 1.3.2 and 1.4.2 <http://symfony.com/blog/symfony-1-3-2-and-1-4-2>`_
* 27 aprile 2009: `symfony 1.2.6: Security fix <http://symfony.com/blog/symfony-1-2-6-security-fix>`_
* 3 ottobre 2008: `symfony 1.1.4 released: Security fix <http://symfony.com/blog/symfony-1-1-4-released-security-fix>`_
* 14 maggio 2008: `symfony 1.0.16 is out  <http://symfony.com/blog/symfony-1-0-16-is-out>`_
* 1 aprile 2008: `symfony 1.0.13 is out  <http://symfony.com/blog/symfony-1-0-13-is-out>`_
* 21 marzo 2008: `symfony 1.0.12 is (finally) out ! <http://symfony.com/blog/symfony-1-0-12-is-finally-out>`_
* 25 giugno 2007: `symfony 1.0.5 released (security fix) <http://symfony.com/blog/symfony-1-0-5-released-security-fix>`_

.. _repository Git: https://github.com/symfony/symfony
.. _blog: http://symfony.com/blog/
.. _Security Advisories: http://symfony.com/blog/category/security-advisories
