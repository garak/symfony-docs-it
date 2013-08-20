.. index::
    single: Riferimento configurazione; Classe Kernel

Configurare il Kernel (p.e. AppKernel)
======================================

Si possono impostare alcune configurazioni nella classe kernel stessa (solitamente
``app/AppKernel.php``). Ciò è possibile sovrascrivendo metodi specifici nella
classe padre :class:`Symfony\\Component\\HttpKernel\\Kernel`.

Configurazione
--------------

* `Charset`_
* `Kernel Name`_
* `Root Directory`_
* `Cache Directory`_
* `Log Directory`_

Charset
~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``UTF-8``

Restituisce il set di caratteri usato nell'applicaizone. Per cambiarlo, sovrascrivere il metodo
:method:`Symfony\\Component\\HttpKernel\\Kernel::getCharset` e restituire un set di
caratteri diverso, per esempio::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function getCharset()
        {
            return 'ISO-8859-1';
        }
    }

Kernel Name
~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``app`` (cioè il nome della cartella che contiene la classe Kernel)

Per cambiare questa impostazione, sovrascrivere il metodo :method:`Symfony\\Component\\HttpKernel\\Kernel::getName`.
In alternativa, spostare il kernel in una cartella diversa. Per esempio,
se si posta il kernel in una cartella ``pippo`` (invece di ``app``), il nome del
kernel sarà ``pippo``.

Il nome del kernel solitamente non è importante: è usato nella generazione
dei file di cache. Se si ha un'applicazione con più kernel,
il modo più facile di avere un nome univoco per ciascuno è duplicare la cartella ``app``
e rinominarla in qualche altro modo (p.e. ``pippo``).

Root Directory
~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: la cartella di ``AppKernel``

Restituisce la cartella radice del kernel. Se si usa Symfony Standard
Edition, la cartella radice punta alla cartella ``app``.

Per cambiare questa impostazione, sovrascrivere il metodo
:method:`Symfony\\Component\\HttpKernel\\Kernel::getRootDir`::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        // ...

        public function getRootDir()
        {
            return realpath(parent::getRootDir().'/../');
        }
    }

Cache Directory
~~~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``$this->rootDir/cache/$this->environment``

Restituisce il percorso della cartella della cache. Per cambiarlo, sovrascrivere il metodo
:method:`Symfony\\Component\\HttpKernel\\Kernel::getCacheDir`. Leggere
":ref:`override-cache-dir`" per maggiori informazioni.

Log Directory
~~~~~~~~~~~~~

**tipo**: ``stringa`` **predefinito**: ``$this->rootDir/logs``

Restituisce il percorso della cartella dei log. Per cambiarlo, sovrascrivere il metodo
:method:`Symfony\\Component\\HttpKernel\\Kernel::getLogDir`. Leggere
":ref:`override-logs-dir`" per maggiori informazioni.
