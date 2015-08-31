.. index::
   single: Doctrine; Estensioni comuni

Estensioni di Doctrine: Timestampable: Sluggable, Translatable, ecc.
====================================================================

Doctrine2 è molto flessibile e la comunità ha già creato una serie di utili
estensioni di Doctrine, per aiutare nei compiti più comuni relativi alle entità.

In particolare, la libreria `DoctrineExtensions`_ fornisce integrazione con una
libreria di estensioni, che offre i comportamenti `Sluggable`_, `Translatable`_,
`Timestampable`_, `Loggable`_, `Sortable`_ e `Tree`_.

L'utilizzo di ogni estensione è spiegato in tale repository.

Tuttavia, per installare e attivare ogni estensione, occorre registrare e attivare un
:doc:`ascoltatore di eventi</cookbook/doctrine/event_listeners_subscribers>`.
Per farlo, si hanno due possibilità:

#. Usare `StofDoctrineExtensionsBundle`_, che integra la libreria di cui sopra.

#. Implementare questi servizi direttamente, seguendo la documentazione per l'integrazione
   con Symfony: `installare le estensioni Gedmo di Doctrine2 in Symfony`_

.. _`DoctrineExtensions`: https://github.com/l3pp4rd/DoctrineExtensions
.. _`StofDoctrineExtensionsBundle`: https://github.com/stof/StofDoctrineExtensionsBundle
.. _`Sluggable`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/sluggable.md
.. _`Translatable`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/translatable.md
.. _`Timestampable`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/timestampable.md
.. _`Loggable`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/loggable.md
.. _`Tree`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/tree.md
.. _`Sortable`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/sortable.md
.. _`installare le estensioni Gedmo di Doctrine2 in Symfony`: https://github.com/l3pp4rd/DoctrineExtensions/blob/master/doc/symfony2.md
