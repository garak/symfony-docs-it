Riferimento per i vincoli di validazione
========================================

.. toctree::
   :maxdepth: 1
   :hidden:

   constraints/NotBlank
   constraints/Blank
   constraints/NotNull
   constraints/Null
   constraints/True
   constraints/False
   constraints/Type

   constraints/Email
   constraints/Length
   constraints/Url
   constraints/Regex
   constraints/Ip
   constraints/Uuid

   constraints/Range

   constraints/EqualTo
   constraints/NotEqualTo
   constraints/IdenticalTo
   constraints/NotIdenticalTo
   constraints/LessThan
   constraints/LessThanOrEqual
   constraints/GreaterThan
   constraints/GreaterThanOrEqual

   constraints/Date
   constraints/DateTime
   constraints/Time

   constraints/Choice
   constraints/Collection
   constraints/Count
   constraints/UniqueEntity
   constraints/Language
   constraints/Locale
   constraints/Country

   constraints/File
   constraints/Image

   constraints/CardScheme
   constraints/Currency
   constraints/Luhn
   constraints/Iban
   constraints/Isbn
   constraints/Issn

   constraints/Callback
   constraints/Expression
   constraints/All
   constraints/UserPassword
   constraints/Valid

Il validatore è progettato per validare oggetto in base a *vincoli*.
Nella vita reale, un vincolo potrebbe essere: "la torta non deve essere bruciata". In
Symfony, i vincoli sono simili: sono asserzioni che una condizione sia
vera.

Vincoli supportati
------------------

I seguenti vincoli sono disponibili nativamente in Symfony:

.. include:: /reference/constraints/map.rst.inc
