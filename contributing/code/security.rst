Problemi di sicurezza
=====================

Questo documento spiega la gestione da parte della squadra di Symfony  dei problemi di sicurezza
di Symfony (In cui "Symfony" è il codice ospitato nel `repository Git`_ ``symfony/symfony``.


Segnalare un problema di sicurezza
==================================

Se si è trovato un problema di sicurezza in Symfony, non utilizzare la
lista o il bug tracker e non diffonderlo pubblicamente. Tutte le questioni di
sicurezza devono essere inviate a **security [at] symfony-project.com**. Le email
inviate a questo indirizzo verranno inoltrate alla squadra di sviluppo di Symfony .

Processo di risoluzione
-----------------------

Per ogni rapporto, prima si cercherà di confermare la vulnerabilità. Quando
confermata, la squadra di sviluppo lavorerà a una soluzione seguendo questi passi:

1. Inviare un riconoscimento al segnalatore;
2. Lavorare su una patch;
3. Scrivere un annuncio sul `blog`_ di Symfony, che descriva la vulnerabilità.
   Tale post dovrebbe contenere le seguenti informazioni:

   * un titolo che includa sempre la stringa "Security release";
   * una descrizione della vulnerabilità;
   * le versioni afflitte;
   * i possibili exploit;
   * come applicare patch/aggiornamenti/workaround alle applicazioni afflitte;
   * riconoscimenti.
4. Inviare patch e annuncio al segnalante per una revisione;
5. Applicare la patch a tutte le versioni di Symfony in manutenzione;
6. Pacchettizzare nuove versioni per tutte le versioni afflitte;
7. Pubblicare il post sul `blog`_ ufficiale di Symfony (va anche aggiunti alla
   categoria "`Security Advisories`_");
8. Aggiornare la lista degli avvisi di sicurezza (vedere sotto).

.. note::

    I rilasci che includono questioni di sicurezza non andrebbero fatti di sabato o
    domenica, a meno che la vulnerabilità non sia stata resa pubblica.

.. note::

   Mentre la patch è in corso di lavorazione, si prega di non rivelare pubblicamente la problematica.

Bollettini di sicurezza
-----------------------

Questa sezione elenca le vulnerabilità di sicurezza che sono state risolte in Symfony,
partendo da Symfony 1.0.0:

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

.. _repository Git:      https://github.com/symfony/symfony
.. _blog:                https://symfony.com/blog/
.. _Security Advisories: http://symfony.com/blog/category/security-advisories
