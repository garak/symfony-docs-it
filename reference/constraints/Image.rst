Image
=====

Questo vincolo funziona esattamente come :doc:`File</reference/constraints/File>`,
tranne per il fatto che le opzioni `mimeTypes`_ e `mimeTypesMessage` sono impostate
automaticamente per lavorare specificatamente su file di tipo immagine.

Inoltre, da Symfony 2.1, ha delle opzioni che consentono di validare larghezza e
altezza dell'immagine.

Vedere il vincolo :doc:`File</reference/constraints/File>` per la documentazione completa
su questo vincolo.

+----------------+----------------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`                      |
+----------------+----------------------------------------------------------------------------+
| Opzioni        | - `mimeTypes`_                                                             |
|                | - `minWidth`_                                                              |
|                | - `maxWidth`_                                                              |
|                | - `maxHeight`_                                                             |
|                | - `minHeight`_                                                             |
|                | - `mimeTypesMessage`_                                                      |
|                | - `sizeNotDetectedMessage`_                                                |
|                | - `maxWidthMessage`_                                                       |
|                | - `minWidthMessage`_                                                       |
|                | - `maxHeightMessage`_                                                      |
|                | - `minHeightMessage`_                                                      |
|                | - Vedere :doc:`File</reference/constraints/File>` per le opzioni ereditate |
+----------------+----------------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\File`                  |
+----------------+----------------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\FileValidator`         |
+----------------+----------------------------------------------------------------------------+

Uso di base
-----------

Questo vincolo si usa solitamente su una proprietà che sarà resa in un form
come tipo :doc:`file</reference/forms/types/file>`. Per esempio, si
supponga di creare un form per un autore, in cui si può caricare una sua
immagine in primo piano. Nel form, la proprietà ``headshot`` sarebbe di tipo
``file``. La classe ``Author`` potrebbe assomigliare a questa::

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    use Symfony\Component\HttpFoundation\File\File;

    class Author
    {
        protected $headshot;

        public function setHeadshot(File $file = null)
        {
            $this->headshot = $file;
        }

        public function getHeadshot()
        {
            return $this->headshot;
        }
    }

Per assicurare che l'oggetto ``headshot`` sa un'immagine valida e che sia entro
certe dimensioni, aggiungere il seguente:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author
            properties:
                headshot:
                    - Image:
                        minWidth: 200
                        maxWidth: 400
                        minHeight: 200
                        maxHeight: 400


    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Image(
             *     minWidth = 200,
             *     maxWidth = 400,
             *     minHeight = 200,
             *     maxHeight = 400
             * )
             */
            protected $headshot;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="headshot">
                    <constraint name="Image">
                        <option name="minWidth">200</option>
                        <option name="maxWidth">400</option>
                        <option name="minHeight">200</option>
                        <option name="maxHeight">400</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        // ...

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Image;

        class Author
        {
            // ...

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('headshot', new Image(array(
                    'minWidth' => 200,
                    'maxWidth' => 400,
                    'minHeight' => 200,
                    'maxHeight' => 400,
                )));
            }
        }

La proprietà ``headshot`` è validata per assicurare che sia una vera immagine e
e che abbia altezza e larghezza entro i limiti.

Opzioni
-------

Questo vincolo condivide tutte le sue opzioni con il vincolo :doc:`File</reference/constraints/File>`.
Tuttavia, modifica due dei valori predefiniti delle opzioni e
aggiunge diverse altre opzioni:

mimeTypes
~~~~~~~~~

**tipo**: ``array`` o ``stringa`` **predefinito**: ``image/*``

Una lista di tipi mime è disponibile sul `sito web di IANA`_

mimeTypesMessage
~~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This file is not a valid image``

minWidth
~~~~~~~~

**tipo**: ``intero``

Se impostato, la larghezza del file immagine deve essere maggiore o uguale di
questo valore in pixel.

maxWidth
~~~~~~~~

**tipo**: ``intero``

Se impostato, la larghezza del file immagine deve essere minore o uguale di
questo valore in pixel.

minHeight
~~~~~~~~~

**tipo**: ``intero``

Se impostato, l'altezza del file immagine deve essere maggiore o uguale di
questo valore in pixel.

maxHeight
~~~~~~~~~

**tipo**: ``intero``

Se impostato, l'altezza del file immagine deve essere minore o uguale di
questo valore in pixel.

sizeNotDetectedMessage
~~~~~~~~~~~~~~~~~~~~~~

**tipo**: ``string`` **predefinito**: ``The size of the image could not be detected``

Se il sistema non è in grado di determinare la dimensione dell'immagine, sarà
mostrato questo errore. Questo si verificherà solo se almeno uno dei quattro vincoli
di dimensione è stato impostato.

maxWidthMessage
~~~~~~~~~~~~~~~

**tipo**: ``string`` **predefinito**: ``The image width is too big ({{ width }}px). Allowed maximum width is {{ max_width }}px``

Il messaggio di errore se la larghezza dell'immagine eccede `maxWidth`_.

minWidthMessage
~~~~~~~~~~~~~~~

**tipo**: ``string`` **predefinito**: ``The image width is too small ({{ width }}px). Minimum width expected is {{ min_width }}px``

Il messaggio di errore se la larghezza dell'immagine è inferiore a `maxWidth`_.

maxHeightMessage
~~~~~~~~~~~~~~~~

**tipo**: ``string`` **predefinito**: ``The image height is too big ({{ height }}px). Allowed maximum height is {{ max_height }}px``

Il messaggio di errore se l'altezza dell'immagine eccede `maxHeight`_.

minHeightMessage
~~~~~~~~~~~~~~~~

**tipo**: ``string`` **predefinito**: ``The image height is too small ({{ height }}px). Minimum height expected is {{ min_height }}px``

Il messaggio di errore se l'altezza dell'immagine è inferiore a `maxHeight`_.

.. _`sito web di IANA`: http://www.iana.org/assignments/media-types/image/index.html
