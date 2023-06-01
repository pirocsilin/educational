<html>
  <head>
    <meta http-equiv="content-type" content="text/html; charset=utf-8"/>
    <title></title>
    <style>
      #field{
        width: 40px;
      }
      .step{
        margin-left: 40px;
        margin-top: 40px;
      }
    </style>
  </head>
  <body>
  
  <?php
    function Poisk($n, $k, $i){
      global $Arr, $num, $count;
      if ($n<0) return; 						/*Остановка рекурентных вызовов 
												так как очередное слагаемое не входит
												в очередное разложение */
      
      if ($n==0){      							/*Найдены все слагаемые в очередном
												разложении. Выводим результат и 
												цепочку рекурентных вызовов завершается*/
                        
        print "<tr><td>$count</td><td>$num = ";
        for($j=0;$j<$i;$j++){
          print "$Arr[$j]";
          if($j<$i-1)
            print " + ";
        }
        print "</tr></td>";
        $count++;
      }
      else{
        if($n - $k >= 0){   				//Если условие верно, то имеем очередное слагаемое.
          $Arr[$i] = $k;    				//Помещаем его в результирующий массив.
          
          Poisk($n - $k, $k, $i+1); 		//Уменьшаем $n на найденное слагаемое и запускаем 
        }                           		//новый вызов для вышеописанных проверок.
        
        if($k - 1 > 0){     				/*Если число $к еще можно использовать в качестве
											слагаемоего (к>0), то уменьшаем его и производим
											проверку на вхождениев в остаток от $n*/
          Poisk($n, $k - 1, $i);
        }                    				//Иначе найдены все разложения.
      }
      return;
    }

    print "<div class='step'>";
    if($_GET['n'] <> ""){
      $n = (integer)$_GET['n'];
      if($n>0 && $n<=20){
        $count = 1;
        $Arr = array();
        $num = $n;
        print "<table border=1 cellpadding=5 cellspacing=0 bordercolor='Gray'>";
        print "<caption><b>Разложения числа $num</b></caption>";
        Poisk($n, $n, 0);
        print "</table>";
      }
      else{
        print "<p>Задан не верный формат числа (n=$_GET[n])";
        print "<ul><li>задайте число как  1 <= n <= 20</li></ul>";
      }
    }
  ?>
	
  <form action="" method="GET">
    <p></p>Введиде число <b>n</b> (min 1, max 20):
    &nbsp;&nbsp;<input type="text" name="n" id="field">
    <p><input type="submit" value="Найти разложения">
  </form>
  </div>
  
  </body>
</html>
