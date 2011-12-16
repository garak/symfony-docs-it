@Template
=========

Utilizzo
--------

L'annotazione ``@Template`` associa un controllore con un nome di template::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Template("SensioBlogBundle:Post:show")
     */
    public function showAction($id)
    {
        // get the Post
        $post = ...;

        return array('post' => $post);
    }

Quando si usa l'annotazione ``@Template``, il controllore dovrebbe restituire un array
di parametri da passare alla vista, al posto dell'oggetto ``Response``.

.. tip::
   Se l'azione restituisce un oggetto ``Response``, l'annotazione ``@Template`` 
   viene semplicemente ignorata.

Se il template prende il suo nome da controllore e azione, che è il caso dell'esempio
precedente, si può anche omettere il valore nell'annotazione::

    /**
     * @Template
     */
    public function showAction($id)
    {
        // get the Post
        $post = ...;

        return array('post' => $post);
    }

Inoltre, se gli unici parametri da passare al template sono parametri del metodo, si
può usare l'attributo ``vars``, invece di restituire un array. Questo risulta molto utile
in combinazione con l' :doc:`annotazione <converters>`
``@ParamConverter``::

    /**
     * @ParamConverter("post", class="SensioBlogBundle:Post")
     * @Template("SensioBlogBundle:Post:show", vars={"post"})
     */
    public function showAction(Post $post)
    {
    }

Che equivale, grazie alle convenzioni, alla configurazione seguente::

    /**
     * @Template(vars={"post"})
     */
    public function showAction(Post $post)
    {
    }

Si può sintetizzare ulteriormente, perché tutti i parametri del metodo sono passati
automaticamente al template, se il metodo restituisce ``null`` e nessun attributo ``vars``
è stato definito::

    /**
     * @Template
     */
    public function showAction(Post $post)
    {
    }

