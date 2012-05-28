.. index::
   single: Bundle; Ereditarietà

Come sovrascrivere parti di un bundle
=====================================

Questa ricetta è un riferimento veloce su come sovrascrivere le varie parti di un
bundle di terze parti.

Template
--------

Per informazioni su come sovrascrivere i template, vedere
* :ref:`overriding-bundle-templates`.
* :doc:`/cookbook/bundles/inheritance`

Rotte
-----

Le rotte non sono mai importate automaticamente in Symfony2. Se si vogliono includere rotte
da un bundle, occorre importarle manualmente da qualche parte nella
propria applicazione (p.e. ``app/config/routing.yml``).

Il modo migliore per "sovrascrivere" le rotte di un bundle è quello di non importarle
affatto. Invece di importare le rotte di un bundle di terze parti, meglio copiare
il file delle rotte nella propria applicazione, modificarlo e importare quello.

Controllori
-----------

Ipotizzando che un bundle di terze parti usi controllori che non siano servizi (che
è quasi sempre il caso), si possono facilmente sovrascrivere tramite l'ereditarietà
dei bundle. Per maggiori informazioni, vedere :doc:`/cookbook/bundles/inheritance`.

Servizi & configurazione
------------------------

In corso...

Entità & mappature
------------------

In corso...

Form
----

In corso...

Meta-dati di validazione
------------------------

In corso...

Traduzioni
----------

In corso...