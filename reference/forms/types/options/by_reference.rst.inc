by_reference
~~~~~~~~~~~~

**tipo**: ``Booleano`` **predefinito**: ``true``

Se si ha un campo ``nome``, ci si aspetta che ``setNome()`` venga invocato
sull'oggetto sottostante. In alcuni casi, tuttavia,  ``setNome()`` potrebbe
*non* essere chiamato.  Impostare ``by_reference`` assicura che il metodo ``setNome()``
sia sempre richiamato.

Per spiegare ciò, ecco un semplice esempio::

    $builder = $this->createFormBuilder($article);
    $builder
        ->add('title', 'text')
        ->add(
            $builder->create('author', 'form', array('by_reference' => ?))
                ->add('name', 'text')
                ->add('email', 'email')
        )

Se ``by_reference`` è ``true``, il codice che segue viene eseguito dietro le quinte quando si 
invoca il metodo ``submit()`` (oppure ``handleRequest()``) sul form::

    $article->setTitle('...');
    $article->getAuthor()->setName('...');
    $article->getAuthor()->setEmail('...');

Notare che ``setAuthor()`` non è invocato. L'autore è modificato per riferimento.

Se si configura ``by_reference`` a ``false``, all'invio dei dati viene eseguito il seguente codice::

    $article->setTitle('...');
    $author = $article->getAuthor();
    $author->setName('...');
    $author->setEmail('...');
    $article->setAuthor($author);

Come si vede, quello che ``by_reference=false`` veramente implica è forzare il framework a
chiamare il setter sull'oggetto relativo.

Similmente, se si usa il tipo :doc:`collection </reference/forms/types/collection>`,
dove i tipi di dati nella collection sono oggetti (come per esempio gli ``ArrayCollection`` di Doctrine),
``by_reference`` deve essere configurata a  ``false``, se si vuole che il setter
(p.e. ``setAuthors()``) sia richiamato.
