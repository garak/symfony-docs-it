pattern
~~~~~~~

**tipo**: ``stringa``

Questa opzione è rilevante solo se `widget`_ è impostato a ``choice``.
Lo schema predefinito è basato sull'opzione `format`_ e prova a combaciare
con i caratteri ``M``, ``d`` e ``y`` nello schema del formato. Se non viene trovata
corrispondenza, il valore predefinito è la stringa ``{{ year }}-{{ month }}-{{ day }}``.
I possibili segnaposto di questa opzione sono:

  * ``{{ year }}``: Sostituito con ``year``
  * ``{{ month }}``: Sostituito con ``month``
  * ``{{ day }}``: Sostituito con ``day``
