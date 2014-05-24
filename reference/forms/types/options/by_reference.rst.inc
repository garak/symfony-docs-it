by_reference
~~~~~~~~~~~~

**tipo**: ``Booleano`` **predefinito**: ``true``

Se il valore sottostante a un campo è un oggetto e questa opzione è
``true``, l'oggetto risultante non sarà effettivamente impostato al bind del
form. Per esempio, se si ha un campo ``author`` nell'oggetto sottostante,
istanza di una classe ``Author``, allora se ``by_reference`` è
``false``, l'oggetto ``Author`` sarà aggiornato con i dati sottostanti,
ma ``setAuthor`` non sarà effettivamente richiamata sull'oggetto principale. Poiché
l'oggetto ``Author`` è un riferimento, questo fa veramente differenza solo se si ha
nel metodo ``setAuthor``  una logica personalizzata, che si vuol essere sicuri
sia eseguita. In questo caso, impostare questa opzione a ``false``.


Se si ha un campo 'nome' nel modulo, ci si aspetta che setNome() venga invocato.
In alcuni casi tuttavia  setNome()  può non essere chiamato. 
by_reference configurata in modo opportuno provvede a chiamare  il metodo setNome() .

Per spiegare cio vediamo il seguente esempio:

    $builder = $this->createFormBuilder($article);
    $builder
        ->add('title', 'text')
        ->add(
            $builder->create('author', 'form', array('by_reference' => ?))
                ->add('name', 'text')
                ->add('email', 'email')
        )

Se ``by_reference`` e' true, il codice che segue viene eseguito 'dietro le quinte' quando si 
invoca il metodo ``submit()`` (oppure ``handleRequest()``) sul form::

    $article->setTitle('...');
    $article->getAuthor()->setName('...');
    $article->getAuthor()->setEmail('...');

Nota che  ``setAuthor()`` non e' invocato. L'author (l'autore) e' modificato 'by reference' ovvero tramite riferimento.

Se si configura ``by_reference`` a false, alla sottomissione dei dati del form viene eseguito il seguente codice ::

    $article->setTitle('...');
    $author = $article->getAuthor();
    $author->setName('...');
    $author->setEmail('...');
    $article->setAuthor($author);

Come si vede  quello che ``by_reference=false`` veramente implica e' forzare il framework a
chiamare il setter sul parent dell'oggetto.

Similmente, se si usa  :doc:`collection</reference/forms/types/collection>`
dove i tipi di dati nella collection sono oggetti (come per esempio gli ``ArrayCollection`` di Doctrine),
``by_reference`` deve essere configurata a  ``false`` se e' necessario che il setter (es. ``setAuthors()``) sia chiamato.


