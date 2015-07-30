.. index::
    single: Options Resolver
    single: Componenti; OptionsResolver

Il componente OptionsResolver
=============================

    Il Componente OptionsResolver aiuta a configurare gli oggetti con un array
    di opzioni. Esso supporta valori predefiniti, opzioni con vincoli e opzioni pigre.

Installazione
-------------

È possibile installare il componente in due modi differenti:

* :doc:`installarlo tramite Composer </components/using_components>` (``symfony/options-resolver`` su `Packagist`_)
* utilizzare il repository ufficiale Git (https://github.com/symfony/OptionsResolver)

.. include:: /components/require_autoload.rst.inc

Utilizzo
--------

Si immagini di avere una classe ``Mailer`` che ha 2 opzioni: ``host`` e
``password``. Queste opzioni stanno per essere gestite dal Componente 
OptionsResolver.

Innanzitutto, creare la classe ``Mailer``::

    class Mailer
    {
        protected $options;

        public function __construct(array $options = array())
        {
        }
    }

Si potrebbe impostare il valore di ``$options`` direttamente nella proprietà. Invece,
utilizzare la classe :class:`Symfony\\Component\\OptionsResolver\\OptionsResolver`
e lasciare che essa risolva le opzioni tramite la chiamata al metodo
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::resolve`.
I vantaggi di operare in questo modo saranno più ovvi andando avanti::

    use Symfony\Component\OptionsResolver\OptionsResolver;

    // ...
    public function __construct(array $options = array())
    {
        $resolver = new OptionsResolver();

        $this->options = $resolver->resolve($options);
    }

La proprietà ``$options`` è ora un array ben definito, con tutte le opzioni
risolte rese disponibili::

    // ...
    public function sendMail($from, $to)
    {
        $mail = ...;
        $mail->setHost($this->options['host']);
        $mail->setUsername($this->options['username']);
        $mail->setPassword($this->options['password']);
        // ...
    }

Configurare OptionsResolver
---------------------------

Adesso, si provi a utilizzare effettivamente la classe::

    $mailer = new Mailer(array(
        'host'     => 'smtp.example.org',
        'username' => 'user',
        'password' => 'pa$$word',
    ));

In questo momento, si riceverà una 
:class:`Symfony\\Component\\OptionsResolver\\Exception\\InvalidOptionsException`,
la quale informa che le opzioni ``host`` e ``password`` non esistono.
Questo perché è necessario configurare prima l'``OptionsResolver``, in modo che
sappia quali opzioni devono essere risolte.

.. tip::

    Per controllare se un'opzione esiste, si può utilizzare la
    funzione
    :method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::isKnown`.

Una buona pratica è porre la configurazione in un metodo (per esempio
``setDefaultOptions``). Il metodo viene invocato nel costruttore per configurare
la classe ``OptionsResolver``::

    use Symfony\Component\OptionsResolver\OptionsResolver;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    class Mailer
    {
        protected $options;

        public function __construct(array $options = array())
        {
            $resolver = new OptionsResolver();
            $this->configureOptions($resolver);

            $this->options = $resolver->resolve($options);
        }

        protected function configureOptions(OptionsResolverInterface $resolver)
        {
            // ... configura il resolver, come si apprendererà nelle
            // sezioni successive
        }
    }

Impostare valori predefiniti
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Spesso le opzioni hanno un valore predefinito. Lo si può configurare 
richiamando :method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::setDefaults`::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setDefaults(array(
            'username' => 'root',
        ));
    }

Questo aggiunge un'opzione ``username`` con un valore predefinito
``root``. Se l'utente passerà un'opzione ``username``, il suo valore
sovrascriverà quello predefinito. Non occorre configurare ``username`` come
opzione facoltativa.

Opzioni Obbligatorie
--------------------

Supponiamo che l'opzione ``host`` sia obbligatoria: la classe non può funzionare senza
di essa. Si possono impostare le opzioni obbligatorie invocando
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::setRequired`::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        $resolver->setRequired(array('host'));
    }

A questo punto è possible usare la classe senza errori::

    $mailer = new Mailer(array(
        'host' => 'smtp.example.org',
    ));

    echo $mailer->getHost(); // 'smtp.example.org'

Se un'opzione obbligatoria non viene passata, sarà sollevata una
:class:`Symfony\\Component\\OptionsResolver\\Exception\\MissingOptionsException`.


.. tip::

    Per determinare se un'opzione è obbligatoria, si può usare il metodo
    :method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::isRequired`.


Opzioni Facoltative
-------------------

A volte un'opzione può essere facoltativa (per esempio l'opzione ``password`` nella classe
``Mailer``). È possibile configurare queste opzioni invocando
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::setOptional`::


    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setOptional(array('password'));
    }

Le opzioni con valori predefiniti sono sempre impostate come facoltative.

.. tip::

    Se si imposta un'opzione come facoltativa, non si può essere sicuri che sia compresa o meno
    nell'array. Occorre verificarne l'esistenza prima di poterla usare.

    Per evitare di doverla verificare ogni volta, si può anche impostare un valore predefinito di
    ``null``, usando il metodo ``setDefaults()`` (vedere `Impostare valori predefiniti`_),
    il che vuol dire che l'elemento esisterà sempre nell'array, ma con un valore predefinito di
    ``null``.

Valori predefiniti che dipendono da altre Opzioni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Supponiamo di aggiungere un'opzione ``port`` alla classe ``Mailer``, il cui valore predefinito
è indovinato sulla base dell'host. Lo si può fare facilmente, usando una
Closure come valore predefinito::

    use Symfony\Component\OptionsResolver\Options;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setDefaults(array(
            'encryption' => null,
            'port' => function (Options $options) {
                if ('ssl' === $options['encryption']) {
                    return 465;
                }

                return 25;
            },
        ));
    }

La classe :class:`Symfony\\Component\\OptionsResolver\\Options` implementa
:phpclass:`ArrayAccess`, :phpclass:`Iterator` e :phpclass:`Countable`. Ciò vuol
dire che la si può gestire come un normale array che contenga le opzioni.

.. caution::

    Il primo parametro della Closure deve essere di tipo ``Options``,
    altrimenti sarà considerata come il valore.

Sovrascrivere i valori predefiniti
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Un valore predefinito, impostato in precedenza, può essere sovrascritto invocando di nuovo
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::setDefaults`.
Se si usa una closure come nuovo valore, riceverà due parametri:

* ``$options``: un'istanza di :class:`Symfony\\Component\\OptionsResolver\\Options`, 
  con tutti i valori predefiniti
* ``$previousValue``: il precedente valore predefinito

.. code-block:: php

    use Symfony\Component\OptionsResolver\Options;
    use Symfony\Component\OptionsResolver\OptionsResolverInterface;

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...
        $resolver->setDefaults(array(
            'encryption' => 'ssl',
            'host' => 'localhost',
        ));

        // ...
        $resolver->setDefaults(array(
            'encryption' => 'tls', // sovrascrittura semplice
            'host' => function (Options $options, $previousValue) {
                return 'localhost' == $previousValue
                    ? '127.0.0.1'
                    : $previousValue;
            },
        ));
    }

.. tip::

    Se il precedente valore predefinito è calcolato da una closure impegnativa e
    non si ha bisogno di accedervi, si può usare invece il metodo
    :method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::replaceDefaults`.
    Questo metodo agisce come ``setDefaults``, ma cancella semplicemente il
    valore precedente, per migliorare le prestazioni. Questo vuol dire che il precedente
    valore predefinito non è disponibile se si sovrascrive con un'altra closure::

        use Symfony\Component\OptionsResolver\Options;
        use Symfony\Component\OptionsResolver\OptionsResolverInterface;

        // ...
        protected function setDefaultOptions(OptionsResolverInterface $resolver)
        {
            // ...
            $resolver->setDefaults(array(
                'encryption' => 'ssl',
                'heavy' => function (Options $options) {
                    // dei calcoli pesanti per creare $result

                    return $result;
                },
            ));

            $resolver->replaceDefaults(array(
                'encryption' => 'tls', // sovrascrittura semplice
                'heavy' => function (Options $options) {
                    // $previousValue non disponibile
                    // ...

                    return $someOtherResult;
                },
            ));
        }

.. note::

    Le chiavi di opzioni esistenti non menzionate durante la sovrascrittura saranno preseervate.

Configurare i Valori consentiti
-------------------------------

Non tutti i valori sono validi per le opzioni. Supponiamo che la classe ``Mailer`` abbia
un'opzione ``transport``, che può valere solo ``sendmail``, ``mail`` o
``smtp``. È possibile configurare questi valori consentiti, invocando
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::setAllowedValues`::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setAllowedValues(array(
            'encryption' => array(null, 'ssl', 'tls'),
        ));
    }

Esiste anche un metodo
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::addAllowedValues`, 
che è possibile utilizzare se si vuole aggiungere un valore consentito al precedente
set di valori consentiti.

Configurare i Tipi consentiti
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

È possibile anche specificare i valori consentiti. Per esempio, l'opzione ``firstName`` può
essere qualsiasi cosa, ma deve essere una stringa. È possibile configurare questi tipi invocando
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::setAllowedTypes`::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setAllowedTypes(array(
            'port' => 'integer',
        ));
    }

I possibili tipi sono quelli associati alle funzioni php ``is_*`` o al nome
della classe. È possibile passare anche un array di tipi come valore. Per esempio,
``array('null', 'string')`` consente a ``port`` di essere nullo o una stringa.

Esiste anche un metodo
:method:`Symfony\\Component\\OptionsResolver\\OptionsResolver::addAllowedTypes`, 
che può essere utilizzato per aggiungere un tipo consentito a quelli precedentemente indicati.

Normalizzare le Opzioni
-----------------------

Alcuni valori devono essere normalizzati prima che possano essere usati. Per esempio,
``firstName`` dovrebbe sempre iniziare con una lettera maiuscola. Per fare ciò, si posso 
scrivere dei normalizzatori. Queste Closure saranno eseguite dopo che tutte le opzioni sono state
passate e ritornano il valore normalizzato. I normalizzatori possono essere configurati
invocando
:method:`Symfony\\Components\\OptionsResolver\\OptionsResolver::setNormalizers`::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setNormalizers(array(
            'host' => function (Options $options, $value) {
                if ('http://' !== substr($value, 0, 7)) {
                    $value = 'http://'.$value;
                }

                return $value;
            },
        ));
    }

Si può notare che la closure riceve un parametro ``$options``. Qualche volta, è
necessario utilizzare altre opzioni per normalizzare::

    // ...
    protected function setDefaultOptions(OptionsResolverInterface $resolver)
    {
        // ...

        $resolver->setNormalizers(array(
            'host' => function (Options $options, $value) {
                if (!in_array(substr($value, 0, 7), array('http://', 'https://'))) {
                    if ($options['ssl']) {
                        $value = 'https://'.$value;
                    } else {
                        $value = 'http://'.$value;
                    }
                }

                return $value;
            },
        ));
    }

.. _Packagist: https://packagist.org/packages/symfony/options-resolver
