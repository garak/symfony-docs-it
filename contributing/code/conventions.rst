Convenzioni
===========

La documentazione :doc:`standards` descrive gli standard del codice per i progetti Symfony2
e per i bundle interni e di terze parti. Tale documento descrive gli
standard e le convenzioni utilizzate nel nucleo del framework. per renderlo più
coerente e prevedibile. Seguirle anche nel proprio codice è bene, anche se non
è obbligatorio.

Nomi dei metodi
---------------

Quando un oggetto ha molte relazioni "principali" con degli "elementi"
(oggetti, parametri, ...), i nomi dei metodi sono normalizzati:

  * ``get()``
  * ``set()``
  * ``has()``
  * ``all()``
  * ``replace()``
  * ``remove()``
  * ``clear()``
  * ``isEmpty()``
  * ``add()``
  * ``register()``
  * ``count()``
  * ``keys()``

L'utilizzo di questi metodi è consentito solo quando è chiaro che si
tratti di una relazione principale:

* un ``CookieJar`` ha molti oggetti ``Cookie``;

* un servizio ``Container`` ha molti servizi e molti parametri (essendo i servizi la
  relazione principale, usiamo la convenzione per tale relazione);

* una Console ``Input`` ha molti parametri e molte opzioni. Non c'e una
  relazione principale e quindi questa convezione non si applica.

Per le molte relazioni per le quali la convenzione non si applicare, bisogna
invece seguire i seguenti metodi (in cui ``XXX`` è il nome dell'elemento relazionato):

+----------------------+-------------------+
| Relazione principale | Altre relazioni   |
+======================+===================+
| ``get()``            | ``getXXX()``      |
+----------------------+-------------------+
| ``set()``            | ``setXXX()``      |
+----------------------+-------------------+
| n/d                  | ``replaceXXX()``  |
+----------------------+-------------------+
| ``has()``            | ``hasXXX()``      |
+----------------------+-------------------+
| ``all()``            | ``getXXXs()``     |
+----------------------+-------------------+
| ``replace()``        | ``setXXXs()``     |
+----------------------+-------------------+
| ``remove()``         | ``removeXXX()``   |
+----------------------+-------------------+
| ``clear()``          | ``clearXXX()``    |
+----------------------+-------------------+
| ``isEmpty()``        | ``isEmptyXXX()``  |
+----------------------+-------------------+
| ``add()``            | ``addXXX()``      |
+----------------------+-------------------+
| ``register()``       | ``registerXXX()`` |
+----------------------+-------------------+
| ``count()``          | ``countXXX()``    |
+----------------------+-------------------+
| ``keys()``           | n/d               |
+----------------------+-------------------+

.. note::

    Pur essendo "setXXX" e "replaceXXX" molto simili, c'e una differenza:
    "setXXX" può sostituire o aggiungere nuovi elementi alla relazione.
    D'altra parte, "replaceXXX"  non può aggiungere nuovi elementi. Se le viene passata
    una chiave non riconosciuta, "replaceXXX" deve lanciare un'eccezione.
