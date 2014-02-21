Generare una nuova classe Type per un form basato su un'entità Doctrine
=======================================================================

Utilizzo
--------

Il comando ``generate:doctrine:form`` genera una classe type di base per un form, usando
i metadati di mappatura della classe dell'entità fornita:

.. code-block:: bash

    $ php app/console generate:doctrine:form AcmeBlogBundle:Post

Parametri obbligatori
---------------------

* ``entity``: Il nome dell'entità, fornito nella notazione breve, contenente il nome
  del bundle che contiene l'entità e il nome dell'entità stessa. Per esempio:
  ``AcmeBlogBundle:Post``:

    .. code-block:: bash

        $ php app/console generate:doctrine:form AcmeBlogBundle:Post
