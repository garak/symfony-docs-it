File
====

Valida che un valore sia un file valido, che può essere uno dei seguenti:

* Una stringa (o oggetto con metodo ``__toString()``) con un percorso di un file esistente;

* Un oggetto :class:`Symfony\\Component\\HttpFoundation\\File\\File` valido
  (inclusi oggetti della classe :class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile`).

Questo vincolo si usa solitamente in form con il tipo di form
:doc:`file</reference/forms/types/file>`.

.. tip::

    Se il file da validare è un'immagine, provare il vincolo
    :doc:`Image</reference/constraints/Image>`.

+----------------+---------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`               |
+----------------+---------------------------------------------------------------------+
| Opzioni        | - `maxSize`_                                                        |
|                | - `mimeTypes`_                                                      |
|                | - `maxSizeMessage`_                                                 |
|                | - `mimeTypesMessage`_                                               |
|                | - `notFoundMessage`_                                                |
|                | - `notReadableMessage`_                                             |
|                | - `uploadIniSizeErrorMessage`_                                      |
|                | - `uploadFormSizeErrorMessage`_                                     |
|                | - `uploadErrorMessage`_                                             |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\File`           |
+----------------+---------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\FileValidator`  |
+----------------+---------------------------------------------------------------------+

Uso di base
-----------

Questo vincolo si usa solitamente su una proprietà che sarà resa in
un form come tipo di form :doc:`file</reference/forms/types/file>`. Per esempio,
si supponga di aver creato un form autore, in cui si possa caricare un file PDF con
una biografia. In tale form, la proprietà ``bioFile`` è di tipo ``file``.
La classe ``Author`` potrebbe essere come la seguente::

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    use Symfony\Component\HttpFoundation\File\File;

    class Author
    {
        protected $bioFile;

        public function setBioFile(File $file = null)
        {
            $this->bioFile = $file;
        }

        public function getBioFile()
        {
            return $this->bioFile;
        }
    }

Per assicurarsi che l'oggetto ``File`` ``bioFile`` sia valido e che sia al di sotto di
una certa dimensione e un PDF valido, aggiungere il seguente:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                bioFile:
                    - File:
                        maxSize: 1024k
                        mimeTypes: [application/pdf, application/x-pdf]
                        mimeTypesMessage: Please upload a valid PDF

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\File(
             *     maxSize = "1024k",
             *     mimeTypes = {"application/pdf", "application/x-pdf"},
             *     mimeTypesMessage = "Please upload a valid PDF"
             * )
             */
            protected $bioFile;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="bioFile">
                    <constraint name="File">
                        <option name="maxSize">1024k</option>
                        <option name="mimeTypes">
                            <value>application/pdf</value>
                            <value>application/x-pdf</value>
                        </option>
                        <option name="mimeTypesMessage">Please upload a valid PDF</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('bioFile', new Assert\File(array(
                    'maxSize' => '1024k',
                    'mimeTypes' => array(
                        'application/pdf',
                        'application/x-pdf',
                    ),
                    'mimeTypesMessage' => 'Please upload a valid PDF',
                )));
            }
        }

La proprietà ``bioFile`` è validata per garantire che sia un vero file.
Anche la sua dimensione e il suo tipo mime sono validati, perché le opzioni
appropriate sono state specificate.

Opzioni
-------

maxSize
~~~~~~~

**tipo**: ``mixed``

Se impostata, la dimensione del file sottostante deve essere inferiore, per essere
valido. La dimensione del file può essere fornita in uno dei seguenti formati:

* **bytes**: Per specificare ``maxSize`` in byte, passare un valore che sia interamente
  numerico (p.e. ``4096``);

* **kilobytes**: Per specificare ``maxSize`` in kilobyte, passare un numero e un
  suffisso con una "k" minuscola (p.e. ``200k``);

* **megabytes**: Per specificare ``maxSize`` in megabyte, passare un numero e un
  suffisso con una "M" maiuscola (p.e. ``4M``).

mimeTypes
~~~~~~~~~

**tipo**: ``array`` o ``stringa``

Se impostata, il validatore verificherà che il tipo mime del file sottostante
sia uguale al tipo mime dato (se stringa) o esista nell'insieme dei tipi
mime dati (se array).

Si può trovare una lista di tipi mime esistenti sul `sito web di IANA`_

maxSizeMessage
~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``The file is too large ({{ size }}). Allowed maximum size is {{ limit }}``

Messaggio mostrato se il file è più grande dell'opzione `maxSize`_.

mimeTypesMessage
~~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``The mime type of the file is invalid ({{ type }}). Allowed mime types are {{ types }}``

Messaggio mostrato se il tipo mime del file non è un tipo mime valido, in
base all'opzione `mimeTypes`_.

notFoundMessage
~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``The file could not be found``

Messaggio mostrato se non viene trovato alcun file nel percorso fornito. Questo
errore può avvenire solo in caso di valore stringa, perché un oggetto ``File`` non
può essere costruito con un percorso non valido.

notReadableMessage
~~~~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``The file is not readable``

Messaggio mostrato se il file esiste, ma la funzione ``is_readable`` di PHP
fallisce, quando gli si passa il percorso del file.

uploadIniSizeErrorMessage
~~~~~~~~~~~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``The file is too large. Allowed maximum size is {{ limit }}``

Messaggio mostrato se il file caricato è più grande dell'impostazione
``upload_max_filesize`` di php.ini.

uploadFormSizeErrorMessage
~~~~~~~~~~~~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``The file is too large``

Messaggio mostrato se il file caricato è più grande di quanto consentito
dal campo input HTML.

uploadErrorMessage
~~~~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``The file could not be uploaded``

Messaggio mostrato se il file caricato non può essere caricato per una ragione
sconosciuta, per esempio se il file non può essere scritto su
disco.


.. _`sito web di IANA`: http://www.iana.org/assignments/media-types/index.html
