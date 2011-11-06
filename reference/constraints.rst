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
   constraints/MinLength
   constraints/MaxLength
   constraints/Url
   constraints/Regex
   constraints/Ip

   constraints/Max
   constraints/Min

   constraints/Date
   constraints/DateTime
   constraints/Time

   constraints/Choice
   constraints/Collection
   constraints/UniqueEntity
   constraints/Language
   constraints/Locale
   constraints/Country

   constraints/File
   constraints/Image

   constraints/Callback
   constraints/All
   constraints/UserPassword
   constraints/Valid

Il validatore è progettato per validare oggetto in base a *vincoli*.
Nella vita reale, un vincolo potrebbe essere: "la torta non deve essere bruciata". In
Symfony2, i vincoli sono simili: sono asserzioni che una condizione sia
vera.

Vincoli supportati
------------------

I seguenti vincoli sono disponibili nativamente in Symfony2:

.. include:: /reference/constraints/map.rst.inc
