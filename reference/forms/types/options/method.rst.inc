.. versionadded:: 2.3
    L'opzione ``method`` è stata introdotta in Symfony 2.3.

method
~~~~~~

**tipo**: ``stringa`` **predefinito**: ``POST``

Questa opzione specifica il metodo HTTP da usare per il form. Il suo
valore viene reso come attributo ``method`` dell'elemento ``form`` ed è
usato per decidere se processare o meno il form, nel metodo
``handleRequest()``. Valori possibili sono:

* POST
* GET
* PUT
* DELETE
* PATCH

.. note:

    Quando il metodo è PUT, PATCH o DELETE, Symfony aggiungerà automaticamente
    un campo nascossto ``_method`` al form. Tale campo è usato per emulare
    questi metodi HTTP, che non sono supportati dai browser più comuni. Per
    maggiori informazioni, vedere :doc:`/cookbook/routing/method_parameters`.

.. note:

    Solo il metodo PATCH consente l'invio parziale di dati, senza che i
    campi mancanti siano impostati a  ``null`` nel dato sottostante (preservando
    i valori, se presenti).
