# PUC_SOB_PROJ1
Projeto 1 - Sistemas Operacionais B</br>
  Esse projeto visa a criação e teste de um modulo de kernel que interaja com um programa de nivel de usuário e que
realize operações pré definidas de criptografia, descriptografia e cálculo de hash a partir de comandos enviados
pelo programa usuário.

* <b>Instruções:</b>

  Para facilitar o nosso trabalho e evitar que tenhamos problemas de conflito no GIT, seguem algumas pequenas regras:

  * Ao final de cada terça será feito um merge final entre as partes de cada usuário separado e colocado no final, antes disso
qualquer conteudo desse será passado ao backup; Isto é, <b>haverá um merge juntando tudo que foi feito até então.</b>

  * Na criação de variáveis dos códigos para facilitar compreensão por parte dos outros membros, por favor seguir a seguinte nomenclatura, se possivel:
  Variáveis devem ser criadas contendo alguma coisa no nome relacionado ao seu uso
    ao inicio de qualquer função, estrutura ou loop colocar um comentário contendo a explicação acima da 1ª linha descrevendo seu proposito;

  Obrigado pela compreensão, fiquem atentos pois novas regras podem vir a ser escritas a pedido de membros do grupo;
em breve resumos dos links serão adicionados e explicações dos mesmos links serão deixados em pastas espeficicas.

# Como usar o Git pelo Terminal:
* Para pegar o projeto atualizado:</br>
  <b>git pull</b>

* Para atualizar os arquivos:</br>
	<b>git add <i>(para adicionar ao Index o que será enviado)</b></i></br>
	<b>git commit -m "MENSAGEM DO COMMIT" <i>(Para especificar o que mudou)</i></b></br>
	<b>git push <i>(Para enviar a atualização)</i></b></br>


* Caso a mensagem do commit esteja errada:</br>
	<b>git commit --amend</b>


* Criar um novo branch:</br>
	<b>git checkout -b <NOME_BRANCH_NOVO></b>

* Ao criar um novo branch, é necessário executar um "<b>git pull</b>" para atualizar o repositório local


* Para trocar de branch:</br>
	<b>git checkout <NOME_BRANCH></b>
	

* Para trazer as mudanças de um branch para outro:</br>
	<b>git merge <NOME_BRANCH_QUE_SERA_PUXADO_PARA_O_ATUAL></b>
