Convenzioni
===========

La documentazione :doc:`standards` descrive gli standard del codice per i progetti Symfony
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

.. _contributing-code-conventions-deprecations:

Deprecati
---------

A volte, alcuni classi o metodi sono deprecatii ne
framework. Ciò accade quando l'implementazione di una caratteristica non può essere
cambiata, per questioni di retrocompatibilità, ma si vuole comunque proporre
un'alternativa "migliore". In tal caso, la vecchia implementazione può essere
**deprecata**.

Una caratteristica viene segnata come deprecata aggiungendo un'annotazione ``@deprecated`` a
classi, metodi, proprietà, ...::

    /**
     * @deprecated Deprecated since version 2.X, to be removed in 2.Y. Use XXX instead.
     */

Il messaggio dovrebbe indicare la versione in cui la classe/metodo è stata
deprecata, la versione in cui sarà rimossa e, ove possibile, in che modo
la caratteristica è stata rimpiazzata.

Inoltre, bisogna lanciare un errore PHP ``E_USER_DEPRECATED``, per aiutrare gli sviluppatori
nella migrazione, a partire da una o due versioni minori prima della versione in cui la
caratteristica sarà rimossa (a seconda della criticità della rimozione)::

    trigger_error(
        'XXX() is deprecated since version 2.X and will be removed in 2.Y. Use XXX instead.',
        E_USER_DEPRECATED
    );
