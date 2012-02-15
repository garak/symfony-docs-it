Url
===

Valida che un valore sia un valid URL string.

+----------------+---------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`               |
+----------------+---------------------------------------------------------------------+
| Opzioni        | - `message`_                                                        |
|                | - `protocols`_                                                      |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\Url`            |
+----------------+---------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\UrlValidator`   |
+----------------+---------------------------------------------------------------------+

Uso di base
-----------

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                bioUrl:
                    - Url:

    .. code-block:: php-annotations

       // src/Acme/BlogBundle/Entity/Author.php
       namespace Acme\BlogBundle\Entity;
       
       use Symfony\Component\Validator\Constraints as Assert;

       class Author
       {
           /**
            * @Assert\Url()
            */
            protected $bioUrl;
       }

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value is not a valid URL``

Messaggio mostrato se the URL is invalid.

protocols
~~~~~~~~~

**tipo**: ``array`` **predefinito**: ``array('http', 'https')``

The protocols that will be considered to be valid. For example, if you also
needed ``ftp://`` type URLs to be valid, you'd redefine the ``protocols``
array, listing ``http``, ``https``, and also ``ftp``.
