input
~~~~~

**tipo**: ``stringa`` **predefinito**: ``datetime``

Il formato in *ingresso* dei dati, cioè il formato in cui la data è memorizzata
nell'oggetto sottostante. Valori validi sono:

* ``stringa`` (p.e. ``2011-06-05``)
* ``datetime`` (un oggetto ``DateTime``)
* ``array`` (p.e. ``array('year' => 2011, 'month' => 06, 'day' => 05)``)
* ``timestamp`` (p.e. ``1307232000``)

Anche il valore proveniente dal form sarà normalizzato in tale
formato.
