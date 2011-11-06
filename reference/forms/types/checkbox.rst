.. index::
   single: Form; Campi; checkbox

Tipo di campo checkbox
======================

Crea un singolo input di tipo checkbox. Andrebbe sempre usato per un campo con un
valore booleano: se il box è spuntato, il campo sarà impostato a vero, altrimenti
il campo sarà impostato a falso.

+----------------+------------------------------------------------------------------------+
| Reso come      | campo ``input`` ``text``                                               |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `value`_                                                             |
+----------------+------------------------------------------------------------------------+
| Opzioni        | - `required`_                                                          |
| ereditate      | - `label`_                                                             |
|                | - `read_only`_                                                         |
|                | - `error_bubbling`_                                                    |
+----------------+------------------------------------------------------------------------+
| Tipo genitore  | :doc:`field</reference/forms/types/field>`                             |
+----------------+------------------------------------------------------------------------+
| Classee        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\CheckboxType` |
+----------------+------------------------------------------------------------------------+

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
impostato nel proprio oggetto.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`field</reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
