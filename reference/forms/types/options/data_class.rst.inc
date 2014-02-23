data_class
~~~~~~~~~~

**tipo**: ``stringa``

Questa opzione è usata per impostare il data mapper appropriato per l'uso con il form,
in modo che si possa usarlo per qualsiasi tipo di campo che richieda un oggetto.

.. code-block:: php

    $builder->add('media', 'sonata_media_type', array(
        'data_class' => 'Acme\DemoBundle\Entity\Media',
    ));
