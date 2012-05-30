Convenzioni
===========

La documentazione :doc:`standards` descrive gli standard del codice per i progetti Symfony2 e
per i bundle interni e di terze parti. Tale documento descrive gli standard e le convenzioni del
codice utilizzate nel nucleo del framework per renderlo più coerente e prevedibile.
L'utilizzo nel proprio codice è incoraggiato, ma non
obbligatorio.

Nomi dei metodi
---------------

Quando un oggetto ha molte relazioni "principali" correlate a "cose"
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

* un servizio ``Container`` ha molti servizi e molti parametri (come servizi nelle
  relazioni principali, quindi si usa la convenzione);

* una Console di ``Input`` ha molti argomenti e molte opzioni. Non c'e una
  relazione principaleThere is no "main" e quindi questa convezione non è applicata

Per le relazioni per le quali non si può applicare la convenzione, bisogna
invece seguire i seguenti metodi (dove ``XXX`` è il nome della cosa relazionata ):

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
    A "replaceXXX", invece, è espressamente vietato aggiungere nuove
    elementi, ma la maggior parte solleva un'eccezione in questi casi.
