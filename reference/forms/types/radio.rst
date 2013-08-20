.. index::
   single: Form; Campi; radio

Tipo di campo radio
===================

Crea un singolo bottone radio. Dovrebbe essere sempre usato per un campo con un valore
booleano: se il radio è selezionato, il campo sarà impostato a ``true``, altrimenti
sarà impostato a ``false``.

Il tipo ``radio`` solitamente non è usato direttamente. Più comunemente, è usato
internamente da altri tipo, come :doc:`choice</reference/forms/types/choice>`.
Se si desidera un campo booleano, usare :doc:`checkbox</reference/forms/types/checkbox>`.

+---------------+---------------------------------------------------------------------+
| Reso come     | campo ``input`` ``radio``                                           |
+---------------+---------------------------------------------------------------------+
| Opzioni       | - `value`_                                                          |
+---------------+---------------------------------------------------------------------+
| Opzioni       | - `required`_                                                       |
| ereditate     | - `label`_                                                          |
|               | - `read_only`_                                                      |
|               | - `disabled`_                                                       |
|               | - `error_bubbling`_                                                 |
|               | - `error_mapping`_                                                  |
|               | - `mapped`_                                                         |
+---------------+---------------------------------------------------------------------+
| Tipo genitore | :doc:`form</reference/forms/types/form>`                            |
+---------------+---------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\RadioType` |
+---------------+---------------------------------------------------------------------+

Opzioni del campo
-----------------

value
~~~~~

**tipo**: ``mixed`` **predefinito**: ``1``

Il valore usato effettivamente come valore del bottone radio. Non ha effetti sul
valore impostato sul proprio oggetto.

Opzioni ereditate
-----------------

Queste opzioni sono ereditate dal tipo :doc:`form</reference/forms/types/form>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc
