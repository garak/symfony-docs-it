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

Messaggio mostrato se l'URL non è valido.

protocols
~~~~~~~~~

**tipo**: ``array`` **predefinito**: ``array('http', 'https')``

I protocolli che saranno considerati validi. Per esempio, se occorre che anche
gli URL ``ftp://`` siano validi, si può ridefinire l'array ``protocols``,
elencando ``http``, ``https`` e ``ftp``.
