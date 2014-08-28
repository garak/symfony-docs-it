Uuid
====

.. versionadded:: 2.5
    Il vincolo Uuid è stato introdotto in Symfony 2.5.

Valida che un valore sia un `identificatore univoco universale (UUID)`_ valido per `RFC 4122`_.
Il formato validato è quello dettato dalle linee guida della RFC, ma se ne può usare uno
più lasci, che accetti UUID non standard accettati da altri sistemi (come PostgreSQL).
Si possono limitare le versioni di UUID, usando una lista bianca.

+----------------+---------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo <validation-property-target>`              |
+----------------+---------------------------------------------------------------------+
| Opzioni        | - `message`_                                                        |
|                | - `strict`_                                                         |
|                | - `versions`_                                                       |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Uuid`           |
+----------------+---------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\UuidValidator`  |
+----------------+---------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: yaml

        # src/UploadsBundle/Resources/config/validation.yml
        Acme\UploadsBundle\Entity\File:
            properties:
                identifier:
                    - Uuid: ~

    .. code-block:: php-annotations

        // src/Acme/UploadsBundle/Entity/File.php
        namespace Acme\UploadsBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class File
        {
            /**
             * @Assert\Uuid
             */
             protected $identifier;
        }

    .. code-block:: xml

        <!-- src/Acme/UploadsBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\UploadsBundle\Entity\File">
                <property name="identifier">
                    <constraint name="Uuid" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/UploadsBundle/Entity/File.php
        namespace Acme\UploadsBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class File
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('identifier', new Assert\Uuid());
            }
        }


Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This is not a valid UUID.``

Messaggio mostrato se la stringa non è un UUID valido.

strict
~~~~~~

**tipo**: ``booleano`` **predefinito**: ``true``

Se questa opzione è ``true``, il vincolo verificherà il formato dell'UUID in base alle regole della
RFC: ``216fff40-98d9-11e3-a5e2-0800200c9a66``. Se invece è ``false``,
consentirà formati alternativi, come:

* ``216f-ff40-98d9-11e3-a5e2-0800-200c-9a66``
* ``{216fff40-98d9-11e3-a5e2-0800200c9a66}``
* ``216fff4098d911e3a5e20800200c9a66``

versions
~~~~~~~~

**tipo**: ``int[]`` **predefinito**: ``[1,2,3,4,5]``

Si può usare questa opzione per consentire solo specifiche `versioni di UUID`_.  Sono valide le versioni 1 - 5.
Si possono anche usare le seguenti costanti di PHP:

* ``Uuid::V1_MAC``
* ``Uuid::V2_DCE``
* ``Uuid::V3_MD5``
* ``Uuid::V4_RANDOM``
* ``Uuid::V5_SHA1``

L'impostazione predefinita consente tutte e cinque le versioni.

.. _`identificatore univoco universale (UUID)`: http://it.wikipedia.org/wiki/UUID
.. _`RFC 4122`: http://tools.ietf.org/html/rfc4122
.. _`versioni di UUID`: http://en.wikipedia.org/wiki/Universally_unique_identifier#Variants_and_versions
