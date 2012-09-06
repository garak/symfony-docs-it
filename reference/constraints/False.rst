False
=====

Valida che un valore sia ``false``. Nello specifico, verifica se il valore sia
esattamente ``false``, esattamente l'intero ``0`` o esattamente la stringa
"``0``".

Vedere anche :doc:`True <True>`.

+----------------+---------------------------------------------------------------------+
| Si applica a   | :ref:`proprietà o metodo<validation-property-target>`               |
+----------------+---------------------------------------------------------------------+
| Opzioni        | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Classe         | :class:`Symfony\\Component\\Validator\\Constraints\\False`          |
+----------------+---------------------------------------------------------------------+
| Validatore     | :class:`Symfony\\Component\\Validator\\Constraints\\FalseValidator` |
+----------------+---------------------------------------------------------------------+

Uso di base
-----------

Il vincolo ``False`` può essere applicato a una proprietà o a un metodo "getter",
ma è usato più comunemente nel secondo caso. Per esempio, si supponga di voler
garantire che una proprietà ``state`` *non* sia in un array dinamico
``invalidStates``. Per prima cosa, creare un metodo "getter"::

    protected $state;

    protectd $invalidStates = array();

    public function isStateInvalid()
    {
        return in_array($this->state, $this->invalidStates);
    }

In questo caso, l'oggetto sottostante è valido solamente se il metodo ``isStateInvalid``
restituisce **false**:

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author
            getters:
                stateInvalid:
                    - "False":
                        message: You've entered an invalid state.

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\False()
             */
             public function isStateInvalid($message = "You've entered an invalid state.")
             {
                // ...
             }
        }

.. caution::

    Usando YAML, assicurarsi di inserire ``False`` tra virgolette (``"False"``),
    altrimenti YAML convertirà questo valore in un booleano.

Opzioni
-------

message
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``This value should be false``

Messaggio mostrato se i dati sottostanti non sono ``false``.
