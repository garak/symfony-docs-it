widget
~~~~~~

**tipo**: ``stringa`` **predefinito**: ``choice``

Il modo di base in cui questo campo andrebbe reso. Può essere uno tra:

* ``choice``: rende tre campi select. L'ordine dei select è definito nell'opzione
  `pattern`_.

* ``text``: rende tre campi testuali (mese, giorno, anno).

* ``single_text``: rende un singolo campo testuale. I dati inseriti dall'utente sono
  validati in base all'opzione `format`_.
