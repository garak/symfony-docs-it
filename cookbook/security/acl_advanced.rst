.. index::
   single: Sicurezza; Concetti avanzati su ACL

Concetti avanzati su ACL
========================

Lo scopo di questa ricetta è approfondire il sistema ACL, nonché spiegare alcune
decisioni progettuali che ne stanno alle spalle.

Concetti progettuali
--------------------

Le capacità di sicurezza degli oggetti di Symfony2 sono basate sul concetto di
Access Control List. Ogni **istanza** di un oggetto del dominio ha la sua ACL.
L'istanza ACL contiene una lista dettagliata di Access Control Entry (ACE), usate
per prendere decisioni sugli accessi. Il sistema ACL di Symfony2 si focalizza su
due obiettivi principali:

- fornire un modo per recuperare in modo efficiente grosse quantità di ACL/ACE per gli
  oggetti del dominio e modificarli;
- fornire un modo per prendere facilmente decisioni sulla possibilità o meno che qualcuno
  esegua un'azione su un oggetto del dominio.

Come indicato nel primo punto, una delle capacità principali del sistema ACL di Symfony2
è il modo altamente performante con cui recupera ACL e ACE. Questo è molto importante,
perché ogni ACL può avere tanti ACE, nonché ereditare da altri ACL, come in un
albero. Quindi, non ci appoggiamo specificatamente ad alcun ORM, ma l'implementazione
predefinita interagisce con la connessione usando direttamente il DBAL di
Doctrine.

Identità degli oggetti
~~~~~~~~~~~~~~~~~~~~~~

Il sistema ACL è interamente disaccoppiato dagli oggetti del dominio. Non devono nemmeno
essere nella stessa base dati o nello stesso server. Per ottenere tale
disaccoppiamento, nel sistema ACL gli oggetti sono rappresentati tramite oggetti identità
di oggetti. Ogni volta che si vuole recuperare l'ACL per un oggetto
del dominio, il sistema ACL creerà prima un oggetto identità a partire dall'oggetto del
dominio, quindi passerà tale oggetto identità al fornitore ACL per ulteriori
processi.

Identità di sicurezza
~~~~~~~~~~~~~~~~~~~~~

È analoga all'identità degli oggetti, ma rappresenta un utente o un ruolo
nell'applicazione. Ogni ruolo, o utente, ha la sua identità di sicurezza.

.. versionadded:: 2.5
    Per gli utenti, l'identità di sicurezza si basa sul nome utente. Questo vuol dire che,
    se per qualsiasi ragione il nome di un utente cambia, ci si deve assicurare che
    anche la sua identità di sicurezza venga aggiornata. Il metodo
    :method:`MutableAclProvider::updateUserSecurityIdentity() <Symfony\\Component\\Security\\Acl\\Dbal\\MutableAclProvider::updateUserSecurityIdentity>`
    serve per gestire l'aggiornamento ed è stato introdotto in Symfony 2.5.

Struttura delle tabelle della base dati 
---------------------------------------

L'implementazione predefinita usa cinque tabelle della base dati, elencate sotto. Le
tabelle sono ordinate dalla più piccola alla più grande, in una tipica applicazione:

- *acl_security_identities*: questa tabella registra tutte le identità di sicurezza (SID)
  che contengono ACE. L'implementazione predefinita ha due identità di
  sicurezza:
  :class:`Symfony\\Component\\Security\\Acl\\Domain\\RoleSecurityIdentity` e
  :class:`Symfony\\Component\\Security\\Acl\\Domain\\UserSecurityIdentity`.
- *acl_classes*: questa tabella mappa i nomi delle classi con identificatori univoci, a
  cui possono fare riferimento le altre tabelle
- *acl_object_identities*: ogni riga in questa tabella rappresebta un'istanza di un
  singolo oggetto del dominio
- *acl_object_identity_ancestors*: questa tabella consente di determinare tutti gli
  antenati di un ACL in modo molto efficiente
- *acl_entries*: questa tabella contiene tutti gli ACE. Questa è tipicamente la tabella
  con più righe. Può contenerne decine di milioni, senza impattare in modo significativo
  sulle prestazioni

Visibilità degli Access Control Entry
-------------------------------------

Gli ACE possono avere visibilità diverse in cui applicarsi. In
Symfony2, ci sono di base due visibilità:

- di classe: questi ACE si applicano a tutti gli oggetti della stessa classe
- di oggetto: questa è l'unica visibilità usata nel capitolo precedente e si applica
  solo a uno specifico oggetto

A volte, si avrà bisogno di applicare un ACE solo a uno specifico campo dell'oggetto.
Si supponga di volere che l'ID sia visibile da un amministratore, ma non dal servizio
clienti. Per risolvere questo problema comune, abbiamo aggiunto altre due
sotto-visibilità:

- di campo di classe: questi ACE si applicano a tutti gli oggetti della stessa classe,
  ma solo a un campo specifico
- di campo di oggetto: questi ACE si applicano a uno specifico oggetto e solo a uno
  specifico campo di tale oggetto

Decisioni pre-autorizzazione
----------------------------

Per decisioni pre-autorizzazione, che sono decisioni da prendere prima dell'invocazione
di un metodo o di un'azione sicura, ci si appoggia sul servizio ``AccessDecisionManager``,
usato anche per prendere decisioni di autorizzazione sui ruoli. Proprio come i ruoli,
il sistema ACL aggiunge molti nuovi attributi, che possono essere usati per verificare
diversi permessi.

Mappa predefinita dei permessi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

+------------------+----------------------------+-----------------------------+
| Attributo        | Significato inteso         | Maschere di bit             |
+==================+============================+=============================+
| VIEW             | Se è consentito vedere     | VIEW, EDIT, OPERATOR,       |
|                  | l'oggetto del dominio      | MASTER o OWNER              |
+------------------+----------------------------+-----------------------------+
| EDIT             | Se è consentito modificare | EDIT, OPERATOR, MASTER      |
|                  | l'oggetto del dominio      | o OWNER                     |
|                  |                            |                             |
+------------------+----------------------------+-----------------------------+
| CREATE           | Se è consentito creare     | CREATE, OPERATOR, MASTER    |
|                  | l'oggetto del dominio      | o  OWNER                    |
|                  |                            |                             |
+------------------+----------------------------+-----------------------------+
| DELETE           | Se è consentito eliminare  | DELETE, OPERATOR, MASTER    |
|                  | l'oggetto del dominio      | o  OWNER                    |
|                  |                            |                             |
+------------------+----------------------------+-----------------------------+
| UNDELETE         | Se è consentito            | UNDELETE, OPERATOR, MASTER  |
|                  | ripristinare un precedente | o OWNER                     |
|                  | oggetto del dominio        |                             |
+------------------+----------------------------+-----------------------------+
| OPERATOR         | Se è consentito eseguire   | OPERATOR, MASTER o OWNER    |
|                  | tutte le azioni precedenti |                             |
|                  |                            |                             |
+------------------+----------------------------+-----------------------------+
| MASTER           | Se è consentito eseguire   | MASTER o OWNER              |
|                  | tutte le azioni precedenti |                             |
|                  | e inoltre è consentito     |                             |
|                  | concedere uno dei permessi |                             |
|                  | precedenti ad altri        |                             |
+------------------+----------------------------+-----------------------------+
| OWNER            | Se si possiede l'oggetto   | OWNER                       |
|                  | del dominio. Il            |                             |
|                  | proprietario può eseguire  |                             |
|                  | tutte le azioni precedenti |                             |
|                  | e concedere i permessi     |                             |
|                  | master e owner             |                             |
+------------------+----------------------------+-----------------------------+

Attributi dei permessi o maschere di bit dei permessi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Gli attributi sono usati da ``AccessDecisionManager``, così come i ruoli sono
attributi usati da ``AccessDecisionManager``. Spesso, tali attributi rappresentano di
fatto un aggregato di maschere di bit. Le maschere di bit, d'altro
canto, sono usate internamente dal sistema ACL  per memorizzare in modo efficiente i
permessi degli utenti sulla base dati e verificare gli accessi, usando operazioni di bit molto veloci.

Estensibilità
~~~~~~~~~~~~~

La mappa dei permessi vista sopra non è affatto statica e in teoria può essere
sostituita totalmente. Tuttavia, dovrebbe essere in grado di coprire la maggior parte dei
problemi che si incontrano e, per interoperabilità con altri bundle, si raccomanda di
mantenere i significati che gli abbiamo attribuito.

Decisioni post-autorizzazione
-----------------------------

Le decisioni post-autorizzazione sono eseguite dopo che un metodo sicuro è stato
invocato e coinvolgono solitamente oggetti del dominio restituiti da tali metodi.
Dopo l'invocazione, i fornitori consentono anche di modificare o filtrare gli oggetti
del dominio, prima che siano restituiti.

A causa di limitazioni del linguaggio PHP, non ci sono capacità di post-autorizzazione
predefinite nel componente della sicurezza.
Tuttavia, c'è un bundle sperimentale, JMSSecurityExtraBundle_, che aggiunge tali
capacità. Si veda la documentazione del bundle per maggiori informazioni sulla loro
implementazione.

Processo di determinazione dell'autorizzazione
----------------------------------------------

La classe ACL fornisce due metodi per determinare se un'identità di sicurezza abbia
i bit richiesti, ``isGranted`` e ``isFieldGranted``. Quando l'ACL riceve una richiesta
di autorizzazione tramite uno di questi metodi, delega la
richiesta a un'implementazione di
:class:`Symfony\\Component\\Security\\Acl\\Domain\\PermissionGrantingStrategy`.
Questo consente di sostituire il modo in cui sono prese le decisioni di accesso, senza
dover modificare la classe ACL stessa.

``PermissionGrantingStrategy`` verifica prima tutti gli ACE con visibilità di oggetto. Se
nessuno è applicabile, verifica gli ACE con visibilità di classe. Se nessuno è applicabile,
il processo viene ripetuto con gli ACE dell'ACL genitore. Se non esiste alcun ACL genitore,
viene sollevata un'eccezione.

.. _JMSSecurityExtraBundle: https://github.com/schmittjoh/JMSSecurityExtraBundle
