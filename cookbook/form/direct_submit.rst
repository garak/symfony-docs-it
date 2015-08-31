.. index::
   single: Form; Form::submit()

Usare la funzione submit() per gestire l'invio di form
======================================================

.. versionadded:: 2.3
    Il metodo :method:`Symfony\\Component\\Form\\FormInterface::handleRequest`
    è stato aggiunto in Symfony 2.3.

Con il metodo ``handleRequest``, la gestione dell'invio di form
è veramente facile::

    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function newAction(Request $request)
    {
        $form = $this->createFormBuilder()
            // ...
            ->getForm();

        $form->handleRequest($request);

        if ($form->isValid()) {
            // eseguire qualche azione...

            return $this->redirect($this->generateUrl('task_success'));
        }

        return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
            'form' => $form->createView(),
        ));
    }

.. tip::

    Per approfondire questo metodo, leggere :ref:`book-form-handling-form-submissions`.

.. _cookbook-form-call-submit-directly:

Richiamare Form::submit() a mano
--------------------------------

.. versionadded:: 2.3
    Prima di Symfony 2.3, il metodo ``submit()`` si chiamava ``bind()``.

In alcuni casi, si desidera un maggior controllo sull'esatto invio del form e su quali
dati siano passati. Invece di usare il metodo
:method:`Symfony\\Component\\Form\\FormInterface::handleRequest`
method, si possono passare i dati inviati direttamente a
:method:`Symfony\\Component\\Form\\FormInterface::submit`::

    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function newAction(Request $request)
    {
        $form = $this->createFormBuilder()
            // ...
            ->getForm();

        if ($request->isMethod('POST')) {
            $form->submit($request->request->get($form->getName()));

            if ($form->isValid()) {
                // eseguire qualche azione...

                return $this->redirect($this->generateUrl('task_success'));
            }
        }

        return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
            'form' => $form->createView(),
        ));
    }

.. tip::

    I form con campi innestati si aspettano un array in
    :method:`Symfony\\Component\\Form\\FormInterface::submit`. Si possono anche inviare
    singoli campi, richiamando :method:`Symfony\\Component\\Form\\FormInterface::submit`
    direttamente sul campo::

        $form->get('firstName')->submit('Fabien');

.. _cookbook-form-submit-request:

Passare Request a Form::submit() (deprecato)
--------------------------------------------

.. versionadded::
    Prima di Symfony 2.3, il metodo ``submit`` era noto come ``bind``.

Prima di Symfony 2.3, il metodo :method:`Symfony\\Component\\Form\\FormInterface::submit`
accettava un oggetto :class:`Symfony\\Component\\HttpFoundation\\Request` come
scorciatoia per l'esempio precedente::

    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function newAction(Request $request)
    {
        $form = $this->createFormBuilder()
            // ...
            ->getForm();

        if ($request->isMethod('POST')) {
            $form->submit($request);

            if ($form->isValid()) {
                // eseguire qualche azione...

                return $this->redirect($this->generateUrl('task_success'));
            }
        }

        return $this->render('AcmeTaskBundle:Default:new.html.twig', array(
            'form' => $form->createView(),
        ));
    }

Si può ancora passare :class:`Symfony\\Component\\HttpFoundation\\Request` direttamente a
:method:`Symfony\\Component\\Form\\FormInterface::submit`, ma ora è
deprecato e sarà rimosso in Symfony 3.0. Si dovrebbe usare invece
:method:`Symfony\\Component\\Form\\FormInterface::handleRequest`.
