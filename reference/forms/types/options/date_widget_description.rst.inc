**tipo**: ``stringa`` **predefinito**: ``choice``

Il modo di base in cui questo campo andrebbe reso. Può essere uno tra:

* ``choice``: rende tre select. L'ordine dei select è definito
  nell'opzione `format`_.

* ``text``: rende tre campi di tipo ``text`` (anno, mese, giorno).

* ``single_text``: rende un singolo campo di tipo ``date``. Il dato inserito
  dall'utente è validato in base all'opzione `format`_.
