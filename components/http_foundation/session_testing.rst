.. index::
   single: HTTP
   single: HttpFoundation, Sessioni

Test con le sessioni
====================

Symfony2 è stato progettato fin dall'inizio con la testabilità in mente. Per rendere
facilmente testabile un codice che usa le sessioni, vengono forniti due diversi mock di
meccanismi di memorizzazione, sia per test unitari che per test funzionali.

È difficile testare codice usando le sessioni reali, perché il flusso dello stato di PHP è
globale e non è possibile avere più sessioni in contemporanea nello stesso processo
PHP.

I mock dei sistemi di memorizzazione simulano il flusso della sessione di PHP senza
effettivamente farne partire una, consentendo il test del codice senza complicazioni. Si
possono anche eseguire istanze multiple nello stesso processo PHP.

I mock dei driver di memorizzazione non leggono o scrivono le funzioni di sistema globali
`session_id()` o `session_name()`. Ci sono dei metodi per la simulazione, in caso sia
necessario:

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::getId`: restituisce
  l'ID di sessione.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::setId`: imposta
  l'ID di sessione.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::getName`: restituisce
  il nome della sessione.

* :method:`Symfony\\Component\\HttpFoundation\\Session\\SessionStorageInterface::setName`: imposta
  il nome della sessione.

Test unitari
------------

Per i test unitari in cui non serve persistere la sessione, basta semplicemente
cambiare il sistema di memorizzazione predefinito con
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\MockArraySessionStorage`::

    use Symfony\Component\HttpFoundation\Session\Storage\MockArraySessionStorage;
    use Symfony\Component\HttpFoundation\Session\Session;

    $session = new Session(new MockArraySessionStorage());

Test funzionali
---------------

Per i test funzionali in cui serve persistere dati di sessione tra processi PHP
separati, basta cambiare il sistema di memorizzazione con
:class:`Symfony\\Component\\HttpFoundation\\Session\\Storage\\MockFileSessionStorage`::

    use Symfony\Component\HttpFoundation\Session\Session;
    use Symfony\Component\HttpFoundation\Session\Storage\MockFileSessionStorage;

    $session = new Session(new MockFileSessionStorage());
