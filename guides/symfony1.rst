Symfony2 per utenti di symfony 1
================================

Applicazioni
------------

In un progetto basato su symfony 1, è frequente avere diverse applicazioni: per
esempio, una per il frontend e una per il backend.

In un progetto basato su Symfony2, occorre creare una sola applicazione
(un'applicazione blog, un'applicazione intranet, ...). La maggior parte delle
volte, se si vuole creare una seconda applicazione, sarebbe meglio creare
un altro progetto e condividere alcuni bundle tra esse.

Se poi si ha bisogno di separare le caratteristiche di frontend e di backend
di alcuni bundle, creare dei sotto-namespace per controller, delle sottocartelle
per i template, configurazioni semantiche diverse, configurazioni di rotte
separate e così via.

.. tip::
   Leggere la definizione di :term:`Progetto`, :term:`Applicazione` e
   :term:`Bundle` nel glossario.
