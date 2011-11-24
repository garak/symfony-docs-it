Image
=====

Questo vincolo funziona esattamente come :doc:`File</reference/constraints/File>`,
tranne per il fatto che le opzioni `mimeTypes`_ e `mimeTypesMessage` sono impostate
automaticamente per lavorare specificatamente su file di tipo immagine.

Vedere il vincolo :doc:`File</reference/constraints/File>` per la documentazione completa
su questo vincolo.

Opzioni
-------

Questo vincolo condivide tutte le sue opzioni con il vincolo :doc:`File</reference/constraints/File>`.
Tuttavia, modifica due dei valori predefiniti delle opzioni:

mimeTypes
~~~~~~~~~

**tipo**: ``array`` o ``stringa`` **predefinito**: un array di tipi mime jpg, gif e png

mimeTypesMessage
~~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This file is not a valid image``
