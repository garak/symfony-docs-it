Generare un nuovo controllore
=============================

Uso
---

Il comando ``generate:controller`` genera un nuovo controllore, inclusi
azioni, test, template e rotte.

Per impostazione predefinita, questo comando gira in modalità interattiva e fa domande per
determinare il nome del bundle, la sua posizione, il suo formato di configurazione e la
sua struttura predefinita:

.. code-block:: bash

    $ php app/console generate:controller

Il comando può essere eseguito in modalità non interattiva, usando l'opzione
``--no-interaction``, senza dimenticare tutte le opzioni necessarie:

.. code-block:: bash

    $ php app/console generate:controller --no-interaction --controller=AcmeBlogBundle:Post

Opzioni disponibili
-------------------

* ``--controller``: Il nome del controllore, fornito con notazione breve, contenente
  il nome del bundle in cui si trova il controllore e il nome del
  bundle. Per esempio: ``AcmeBlogBundle:Post`` (crea un  ``PostController`` 
  dentro al bundle ``AcmeBlogBundle``):

    .. code-block:: bash

        $ php app/console generate:controller --controller=AcmeBlogBundle:Post

* ``--actions``: La lista di azioni da generare nel controllore. Il formato è
  del tipo ``%actionname%:%route%:%template`` (dove ``:%template%``
  è facoltativo):

    .. code-block:: bash

        $ php app/console generate:controller --actions="showPostAction:/article/{id} getListAction:/_list-posts/{max}:AcmeBlogBundle:Post:list_posts.html.twig"

        # oppure
        $ php app/console generate:controller --actions=showPostAction:/article/{id} --actions=getListAction:/_list-posts/{max}:AcmeBlogBundle:Post:list_posts.html.twig

* ``--route-format``: (**annotation**) [valori: yml, xml, php o annotation] 
  Questa opzione determina il formato da usare per le rotte. Il formato predefinito usato
  è ``annotation``:

    .. code-block:: bash

        $ php app/console generate:controller --route-format=annotation

* ``--template-format``: (**twig**) [valori: twig o php] Questa opzione determina
  il formato da usare per i template. Il formato predefinito usato è``twig``:


    .. code-block:: bash

        $ php app/console generate:controller --template-format=twig
