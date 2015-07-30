Promessa di retrocompatibilità
==============================

Assicurare facili aggiornamenti dei progetti è una prorità. Per questo motivo,
Symfony promette retrocompatibilità per tutti i suoi rilasci minori.
Questa strategia è anche nota come  `versionamento semantico`_. In breve,
versionamento semantico vuol dire che solo i rilasci maggiori (come 2.0, 3.0, ecc.) possono
infrangere la retrocompatibilità. I rilasci minori (come 2.5, 2.6, ecc.)
possono introdurre nuove caratteristiche, ma devono farlo senza infrangere l'API esistente
del ramo (2.x nell'esempio).

.. caution::

    Questa promessa è stata introdotta con Symfony 2.3 e quindi non si applica alle
    versioni precedenti.

Tuttavia, la retrocompatibilità ha diversi aspetti. Di fatto, quasi
ogni modifica fatta sul framework può potenzialmente rompere un'applicazione.
Per esempio, se si aggiunge un nuovo metodo a una classe, questo potrebbe rompere un'applicazione
che estende tale classe e ha quello stesso metodo, ma con una firma
diversa.

Inoltre, non tutte le infrazioni alla retrocompatibilità hanno lo stesso impatto sul codice. Mentre
alcune infrazioni richiedono modifiche significative a classi o
architettura, altre possono essere risolte semplicemente cambiando un nome di metodo.

Per questo è stata creata questa pagina. la sezione "Usare il codice di Symfony" dirà
come assicurarsi che un'applicazione non si rompa completamente durante
un aggiornamento a una nuova versione dello stesso ramo.

La seconda sezione, "Lavorare sul codice di Symfony", ha come obiettivo i contributori di Symfony.
Questa sezione elenca regole dettagliate che ogni contributori deve
seguire per assicurare aggiornamenti tranquilli agli utenti.

Usare il codice di Symfony
--------------------------

Se si usa Symfony in un progetto, le linee guida seguenti aiuteranno
ad assicurare aggiornamenti tranquilli di tutti i rilasci minori futuri della
stessa versione di Symfony.

Uso delle interfacce
~~~~~~~~~~~~~~~~~~~~

Tutte le interfacce di Symfony possono essere usate per forzare i tipi. Si possono
anche richiamare tutti i metodi dichiarati. È garantito che non sarà infranto il codice
legato a tali regole.

.. caution::

    Fanno eccezione a questa regola le interfacce con tag ``@internal``. Tali
    interfacce non andrebbero usate o implementate.

Se si vuole implementare un'interfaccia, ci si deve prima assicurare che
sia un'interfaccia API. Si possono riconoscere le interfacce API dal tag ``@api``
nel codice sorgente::

    /**
     * HttpKernelInterface handles a Request to convert it to a Response.
     *
     * @author Fabien Potencier <fabien@symfony.com>
     *
     * @api
     */
    interface HttpKernelInterface
    {
        // ...
    }

Se si implementa un'interfaccia API, la promessa è che il codice non sarà
mai rotto. Le interfacce normali, invece potrebbero essere estese nei rilasci minori,
per esempio con aggiunte di nuovi metodi. Occorre essere pronti ad aggiornare a mano del codice,
se si implementa un'interfaccia normale.

.. note::

    Anche in modifiche che richiedano aggiornamenti manuali, sarà assicurato che
    tali modifiche siano semplici. Ci saranno sempre precise istruzioni
    nel file UPGRADE della cartella radice di Symfony.

La tabella seguente spiega in dettaglio i casi d'uso coperti dalla
promessa di retrocompatibilità:

+---------------------------------------------------+---------------+------------------+
| Caso d'uso                                        | Normale       | API              |
+===================================================+===============+==================+
| **Se...**                                         | **Retrocompatibilità garantita** |
+---------------------------------------------------+---------------+------------------+
| Si forza il tipo tramite interfaccia              | Sì            | Sì               |
+---------------------------------------------------+---------------+------------------+
| Si richiama un metodo                             | Sì            | Sì               |
+---------------------------------------------------+---------------+------------------+
| **Se si implementa l'interfaccia e...**           | **Retrocompatibilità garantita** |
+---------------------------------------------------+---------------+------------------+
| Si implementa un metodo                           | No [1]_       | Sì               |
+---------------------------------------------------+---------------+------------------+
| Si aggiunge un parametro a un metodo implementato | No [1]_       | Sì               |
+---------------------------------------------------+---------------+------------------+
| Si aggiunge un valore predefinito a un parametro  | Yes           | Sì               |
+---------------------------------------------------+---------------+------------------+

.. include:: _api_tagging.rst.inc

Uso delle classi
~~~~~~~~~~~~~~~~

Tutte le classi fornite da Symfony possono essere instanziate e accedute tramite i loro
metodi e proprietà pubblici.

.. caution::

    Classi, proprietà e metodi con tag ``@internal``, come anche
    le classi che si trovano negli spazi dei nomi ``*\\Tests\\``, fanno
    eccezione a questa regola. Sono pensati solo per uso interno e non
    vanno acceduti.

Proprio come per le interfacce, c'è una distinzione tra classi API e classi normali.
Come le interfacce API, le classi API sono contraddistinte da un tag ``@api``::

    /**
     * Request represents an HTTP request.
     *
     * @author Fabien Potencier <fabien@symfony.com>
     *
     * @api
     */
    class Request
    {
        // ...
    }

La differenza tra classi normali e classi API è la garanzia di piena
retrocompatibilità se si estende una classe API e se ne sovrascrivono i metodi. Non
c'è la stessa garanzia per le classi normali, in cui potrebbero, per esempio,
essere aggiunti parametri a metodi. Di conseguenza, la firma di un
metodo sovrascritto non corrisponderebbe più e genererebbe un errore fatale.

.. note::

    Come per le interfacce, eventuali modifiche garantiscono comunque facili
    aggiornamenti. Istruzioni dettagliate saranno fornite nel file UPGRADE
    della cartella radice di Symfony.

In alcuni casi, solo proprietà e metodi specifici hanno il tag ``@api``,
pur trovandosi in classi normali. In questi casi, la piena retrocompatibilità
è garantita per proprietà e metodi (come indicato nella colonna
API qui sotto), ma non per il resto della classe.

Per sicurezza, verificare nella tabella seguente quali casi d'uso siano
coperti dalla promessa di retrocompatibilità:

+--------------------------------------------------------+---------------+------------------+
| Caso d'uso                                             | Normale       | API              |
+========================================================+===============+==================+
| *Se...**                                               | **Retrocompatibilità garantita** |
+--------------------------------------------------------+---------------+------------------+
| Si forza il tipo tramite classe                        | Sì            | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si crea una nuova istanza                              | Sì            | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si estende la classe                                   | Sì            | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si accede a una proprietà pubblica                     | Sì            | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si richiama un metodo pubblico                         | Sì            | Sì               |
+--------------------------------------------------------+---------------+------------------+
| **Se si estende la classe e...**                       | **Retrocompatibilità garantita** |
+--------------------------------------------------------+---------------+------------------+
| Si accede a una proprietà protetta                     | No [1]_       | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si richiama un metodo protetto                         | No [1]_       | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si sovrascrive una proprietà pubblica                  | Sì            | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si sovrascrive una proprietà protetta                  | No [1]_       | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si sovrascrive un metodo pubblico                      | No [1]_       | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si sovrascrive un metodo protetto                      | No [1]_       | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si aggiunge una nuova proprietà                        | No            | No               |
+--------------------------------------------------------+---------------+------------------+
| Si aggiunge un nuovo metodo                            | No            | No               |
+--------------------------------------------------------+---------------+------------------+
| Si aggiunge un parametro a un metodo sovrascritto      | No [1]_       | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si aggiunge un valore predefinito a un parametro       | Sì            | Sì               |
+--------------------------------------------------------+---------------+------------------+
| Si richiama un metodo privato (tramite Reflection)     | No            | No               |
+--------------------------------------------------------+---------------+------------------+
| Si accede a una proprietà privata (tramite Reflection) | No            | No               |
+--------------------------------------------------------+---------------+------------------+

.. include:: _api_tagging.rst.inc

Lavorare sul codice di Symfony
------------------------------

Per chi volesse contribuire a migliorare Symfony, nella prossima sezione saranno
mostrare le regole che assicurano aggiornamenti tranquilli per gli utenti.

Modifica di interfacce
~~~~~~~~~~~~~~~~~~~~~~

Questa tabella indica quali modifiche sono consentite quando si lavora
sulle interfacce di Symfony:

==============================================  ==============  ==============
Tipo di modifica                                Normale         API
==============================================  ==============  ==============
Rimuovere del tutto                             No              No
Cambiare nome o spazio dei nomi                 No              No
Aggiungere interfaccia genitrice                Sì [2]_         Sì [3]_
Rimuovere interfaccia genitrice                 No              No
**Metodi**
Aggiungere metodo                               Sì [2]_         No
Rimuovere metodo                                No              No
Cambiare nome                                   No              No
Spostare nell'interfaccia genitrice             Sì              Sì
Aggiungere parametro senza valore predefinito   No              No
Aggiungere parametro con valore predefinito     Sì [2]_         No
Rimuovere parametro                             Sì [4]_         Sì [4]_
Aggiungere valore predefinito a un parametro    Sì [2]_         No
Rimuovere valore predefinito a un parametro     No              No
Aggiungere tipo forzato a un parametro          No              No
Rimuovere tipo forzato da un parametro          Sì [2]_         No
Cambiare tipo di parametro                      Sì [2]_ [5]_    No
Cambiare tipo restituito                        Sì [2]_ [6]_    No
==============================================  ==============  ==============

Modifica di classi
~~~~~~~~~~~~~~~~~~

Questa tabella indica quali modifiche sono consentite quando si lavora
sulle classi di Symfony:

==================================================  ==============  ==============
Tipo di modifica                                    Normale         API
==================================================  ==============  ==============
Rimuovere del tutto                                 No              No
Rendere finale                                      No              No
Rendere astratta                                    No              No
Cambiare nome o spazio dei nomi                     No              No
Cambiare classe genitrice                           Sì [7]_         Sì [7]_
Aggiungere interfaccia                              Sì              Sì
Rimuovere interfaccia                               No              No
**Proprietà pubbliche**
Add public property                                 Sì              Sì
Rimuovere proprietà pubblica                        No              No
Ridurre visibilità                                  No              No
Spostare in classe genitrice                        Sì              Sì
**Proprietà protette**
Aggiungere proprietà protetta                       Sì              Sì
Rimuovere proprietà protetta                        Sì [2]_         No
Ridurre visibilità                                  Sì [2]_         No
Spostare in classe genitrice                        Sì              Sì
**Proprietà private**
Aggiungere proprietà privata                        Sì              Sì
Rimuovere proprietà privata                         Sì              Sì
**Construttori**
Aggiungere costruttore senza parametri obbligatori  Sì [2]_         Sì [2]_
Rimuovere costruttore                               Sì [2]_         No
Ridurre visibilità di un costruttore pubblico       No              No
Ridurre visibilità di un costruttore protetto       Sì [2]_         No
Spostare in classe genitrice                        Sì              Sì
**Metodi pubblici**
Aggiungere metodo pubblico                          Sì              Sì
Rimuovere metodo pubblico                           No              No
Cambiare nome                                       No              No
Ridurre visibilità                                  No              No
Spostare in classe genitrice                        Sì              Sì
Aggiungere parametro senza valore predefinito       No              No
Aggiungere parametro con valore predefinito         Sì [2]_         No
Rimuovere parametro                                 Sì [4]_         Sì [4]_
Aggiungere valore predefinito a un parametro        Sì [2]_         No
Rimuovere valore predefinito a un parametro         No              No
Aggiungere tipo forzato a un parametro              Sì [8]_         No
Rimuovere tipo forzato da un parametro              Sì [2]_         No
Cambiare tipo di parametro                          Sì [2]_ [5]_    No
Cambiare tipo restituito                            Sì [2]_ [6]_    No
**Metodi protetti**
Aggiungere metodo protetto                          Sì              Sì
Rimuovere metodo protetto                           Sì [2]_         No
Cambiare nome                                       No              No
Ridurre visibilità                                  Sì [2]_         No
Spostare in classe genitrice                        Sì              Sì
Aggiungere parametro senza valore predefinito       Sì [2]_         No
Aggiungere parametro con valore predefinito         Sì [2]_         No
Rimuovere parametro                                 Sì [4]_         Sì [4]_
Aggiungere valore predefinito a un parametro        Sì [2]_         No
Rimuovere valore predefinito a un parametro         Sì [2]_         No
Aggiungere tipo forzato a un parametro              Sì [2]_         No
Rimuovere tipo forzato da un parametro              Sì [2]_         No
Cambiare tipo di parametro                          Sì [2]_ [5]_    No
Cambiare tipo restituito                            Sì [2]_ [6]_    No
**Metodi privati**
Aggiungere metodo privato                           Sì              Sì
Rimuovere metodo privato                            Sì              Sì
Cambiare nome                                       Sì              Sì
Ridurre visibilità                                  Sì              Sì
Aggiungere parametro senza valore predefinito       Sì              Sì
Aggiungere parametro con valore predefinito         Sì              Sì
Rimuovere parametro                                 Sì              Sì
Aggiungere valore predefinito a un parametro        Sì              Sì
Rimuovere valore predefinito a un parametro         Sì              Sì
Aggiungere tipo forzato a un parametro              Sì              Sì
Rimuovere tipo forzato da un parametro              Sì              Sì
Cambiare tipo di parametro                          Sì              Sì
Cambiare tipo restituito                            Sì              Sì
**Metodi statici**
Cmabiare non statico in statico                     No              No
Cmabiare statico in non statico                     No              No
==================================================  ==============  ==============

.. [1] Il codice dell'applicazione potrebbe rompersi in conseguenza di modifiche al codice di Symfony. Tali
       modifiche saranno tuttavia documentate nel file UPGRADE.

.. [2] Va evitato. Se fatto, la modifica deve essere documentata nel file
       UPGRADE.

.. [3] L'interfaccia genitrice aggiunta non deve introdurre metodi che non
       esistono già nell'interfaccia.

.. [4] Si possono rimuovere solo gli ultimi parametri, poiché PHP tralascia
       eventuali parametri in sovrannumero passati a un metodo.

.. [5] Il tipo di parametro può essere cambiato solo con uno compatibile o meno specifico.
       Sono ammesse le seguenti modifiche:

       ===================  ==================================================================
       Tipo originale       Nuovo tipo
       ===================  ==================================================================
       booleano             qualsiasi `tipo scalare`_ con equivalenti `valori booleani`_
       stringa              qualsiasi `tipo scalare`_ od oggetto con equivalenti `valori stringhe`_
       intero               qualsiasi `tipo scalare`_ con equivalenti `valori interi`_
       virgola mobile       qualsiasi `tipo scalare`_ con equivalenti `valori a virgola mobile`_
       classe ``<C>``       qualsiasi superclasse o interfaccia di ``<C>``
       interfaccia ``<I>``  qualsiasi superinterfaccia di ``<I>``
       ===================  ==================================================================

.. [6] Il tipo restituito  può essere cambiato solo con uno compatibile o più specifico.
       Sono ammesse le seguenti modifiche:

       ===================  ==================================================================
       Tipo originale       Nuovo tipo
       ===================  ==================================================================
       boolean              qualsiasi `tipo scalare`_ con equivalenti `valori booleani`_
       string               qualsiasi `tipo scalare`_ or object with equivalent `valori stringhe`_
       integer              qualsiasi `tipo scalare`_ con equivalenti `valori interi`_
       float                qualsiasi `tipo scalare`_ con equivalenti `valori a virgola mobile`_
       array                istanza di ``ArrayAccess``, ``Traversable`` e ``Countable``
       ``ArrayAccess``      array
       ``Traversable``      array
       ``Countable``        array
       classe ``<C>``       qualsiasi sottoclasse di ``<C>``
       interfaccia ``<I>``  qualsiasi sottointerfaccia o classe che implementi ``<I>``
       ===================  ==================================================================

.. [7] Quando si cambia la classe genitrice, la classe genitrice originale deve restare un
       antenato della classe.

.. [8] Un tipo forzato si può aggiungere solo se il passaggio di un valore con tipo diverso
       generava in precedenza un errore fatale.

.. _versionamento semantico: http://semver.org/
.. _tipo scalare: http://php.net/manual/it/function.is-scalar.php
.. _valori booleani: http://php.net/manual/it/function.boolval.php
.. _valori stringhe: http://php.net/manual/it/function.strval.php
.. _valori interi: http://php.net/manual/it/function.intval.php
.. _valori a virgola mobile: http://php.net/manual/it/function.floatval.php
