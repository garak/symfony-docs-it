.. index::
   single: Form; Campi; csrf

Tipo di campo
=============

Il tipo ``csrf`` è un campo nascosto che contiene un token per il CSRF.

+---------------+--------------------------------------------------------------------+
| Reso come     | campo ``input`` ``hidden`` field                                   |
+---------------+--------------------------------------------------------------------+
| Opzioni       | - ``csrf_provider``                                                |
|               | - ``page_id``                                                      |
|               | - ``property_path``                                                |
+---------------+--------------------------------------------------------------------+
| Tipo genitore | ``hidden``                                                         |
+---------------+--------------------------------------------------------------------+
| Classe        | :class:`Symfony\\Component\\Form\\Extension\\Csrf\\Type\\CsrfType` |
+---------------+--------------------------------------------------------------------+

Opzioni del campo
-----------------

csrf_provider
~~~~~~~~~~~~~

**tipo**: ``Symfony\Component\Form\CsrfProvider\CsrfProviderInterface``

L'oggetto ``CsrfProviderInterface`` che dovrebbe generare il token CSRF.
Se non impostato, viene usato il provider predefinito.

intention
~~~~~~~~~

**tipo**: ``stringa``

Un identificatore univoco, facoltativo, usato per generare il token CSRF.

.. include:: /reference/forms/types/options/property_path.rst.inc