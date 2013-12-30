.. index::
   single: Form; Campi; submit

submit Field Type
=================

.. versionadded:: 2.3
    Il tipo ``submit`` è stato aggiunto in Symfony 2.3

A submit button.

+----------------------+----------------------------------------------------------------------+
| Reso come            | tag ``input`` ``submit``                                             |
+----------------------+----------------------------------------------------------------------+
| Opzioni              | - `attr`_                                                            |
| ereditate            | - `disabled`_                                                        |
|                      | - `label`_                                                           |
|                      | - `label_attr`_                                                      |
|                      | - `translation_domain`_                                              |
+----------------------+----------------------------------------------------------------------+
| Tipo genitore        | :doc:`button</reference/forms/types/button>`                         |
+----------------------+----------------------------------------------------------------------+
| Classe               | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\SubmitType` |
+----------------------+----------------------------------------------------------------------+

Il bottone Submit button ha un metodo aggiuntivo
:method:`Symfony\\Component\\Form\\ClickableInterface::isClicked`, che consente
di verificare se questo bottone sia stato usato per inviare il form. Questo è utile
specialmente quando :ref:`un form ha più bottoni submit <book-form-submitting-multiple-buttons>`::

    if ($form->get('save')->isClicked()) {
        // ...
    }

Opzioni ereditate
-----------------

.. include:: /reference/forms/types/options/button_attr.rst.inc

.. include:: /reference/forms/types/options/button_disabled.rst.inc

.. include:: /reference/forms/types/options/button_label.rst.inc

.. include:: /reference/forms/types/options/label_attr.rst.inc

.. include:: /reference/forms/types/options/button_translation_domain.rst.inc
