.. index::
   single: Form; Campi; checkbox

Tipo di campo checkbox
======================

Crea un singolo input di tipo checkbox. Andrebbe sempre usato per un campo con un
valore booleano: se il box è spuntato, il campo sarà impostato a vero, altrimenti
il campo sarà impostato a falso.

+---------------+------------------------------------------------------------------------+
| Reso come     | campo ``input`` ``checkbox``                                           |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `value`_                                                             |
+---------------+------------------------------------------------------------------------+
| Opzioni       | - `data`_                                                              |
| ereditate     | - `empty_data`_                                                        |
|               | - `required`_                                                          |
|               | - `label`_                                                             |
|               | - `label_attr`_                                                        |
|               | - `read_only`_                                                         |
|               | - `disabled`_                                                          |
|               | - `error_bubbling`_                                                    |
|               | - `error_mapping`_                                                     |
|               | - `mapped`_                                                            |
+---------------+------------------------------------------------------------------------+
| Tipo genitore | :doc:`form </reference/forms/types/form>`                              |
+---------------+------------------------------------------------------------------------+
| Classee       | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CheckboxType` |
+---------------+------------------------------------------------------------------------+

Esempio di utilizzo
-------------------

.. code-block:: php

    $builder->add('public', 'checkbox', array(
        'label'     => 'Mostrare questa voce?',
        'required'  => false,
    ));

Opzioni del campo
-----------------

value
~~~~~

**tipo**: ``mixed`` **predefinito**: ``1``

Il valore usato effettivamente per il checkbox. Non ha effetti sul valore
impostato nell'oggetto.

.. caution::

    Per fare in modo che un un checkbox sia già selezionato, impostare l'opzione `data`_ a ``true``.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form </reference/forms/types/form>`:

.. include:: /reference/forms/types/options/data.rst.inc

.. include:: /reference/forms/types/options/empty_data.rst.inc

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
