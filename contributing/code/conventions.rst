Convenzioni
===========

La documentazione :doc:`standards` descrive i coding standard per i progetti Symfony2 e
per i bundles interni e di terze parti. Tale documento descrive i coding standard e le convenzioni
utilizzate nel core del framework per renderlo più consistente e prevedibile.

Nomi dei metodi
------------

Quando un oggetto a molte relazioni "principali" correlate a "cose"
(oggetti, parametri, ...) i nomi dei metodi sono normalizzati in: 

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

L'utilizzo di questi metodi sono permessi solo quando è chiaro che si
tratti di una relazione principale:

* un ``CookieJar`` ha molti oggetti ``Cookie``;

* un service ``Container`` ha molti servizi e molti parametri (come servizi nelle
  relazioni principali, quindi si usa la convenzione);

* una Console di ``Input`` ha molti argomenti e molte opzioni. Non c'e una 
  relazione principaleThere is no "main" e quindi questa convezione non è applicata

Per le relazioni per le quali non si può applicare la naming convention bisogna
invece seguire i seguenti metodi (dove ``XXX`` è il nome della cosa relazionata ):

+----------------+-------------------+
| Main Relation  | Other Relations   |
+================+===================+
| ``get()``      | ``getXXX()``      |
+----------------+-------------------+
| ``set()``      | ``setXXX()``      |
+----------------+-------------------+
| n/a            | ``replaceXXX()``  |
+----------------+-------------------+
| ``has()``      | ``hasXXX()``      |
+----------------+-------------------+
| ``all()``      | ``getXXXs()``     |
+----------------+-------------------+
| ``replace()``  | ``setXXXs()``     |
+----------------+-------------------+
| ``remove()``   | ``removeXXX()``   |
+----------------+-------------------+
| ``clear()``    | ``clearXXX()``    |
+----------------+-------------------+
| ``isEmpty()``  | ``isEmptyXXX()``  |
+----------------+-------------------+
| ``add()``      | ``addXXX()``      |
+----------------+-------------------+
| ``register()`` | ``registerXXX()`` |
+----------------+-------------------+
| ``count()``    | ``countXXX()``    |
+----------------+-------------------+
| ``keys()``     | n/a               |
+----------------+-------------------+

.. note::

    Mentre "setXXX" e "replaceXXX" sono molto simili, c'e una differenza:
    "setXXX" può sostituire o aggiungere nuovi elemento alla relazione.
    "replaceXXX" d'altra parte espressamente limitata ad aggiungere nuove
    elementi ed inoltre dovrebbe lanciare eccezioni in quei casi.