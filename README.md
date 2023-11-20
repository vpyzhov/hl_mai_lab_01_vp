
```plantuml
@startuml
!include https://raw.githubusercontent.com/plantuml-stdlib/C4-PlantUML/master/C4_Container.puml

AddElementTag("microService", $shape=EightSidedShape(), $bgColor="CornflowerBlue", $fontColor="white", $legendText="microservice")
AddElementTag("storage", $shape=RoundedBoxShape(), $bgColor="lightSkyBlue", $fontColor="white")

Person(admin, "Администратор")
Person(user, "Пользователь")

System_Ext(web_site, "Клиентский веб-сайт", "HTML, CSS, JavaScript, React", "Веб-интерфейс")

System_Boundary(conference_site, "Магазин") {
   Container(service_user, "Сервис авторизации", "C++", "Сервис управления пользователями", $tags = "microService")    
   Container(service_good, "Сервис товаров", "C++", "Сервис управдления товарами", $tags = "microService") 
   Container(service_cart, "Сервис корзины", "C++", "Сервис управления корзиной пользователя", $tags = "microService")  
   ContainerDb(db, "База данных", "MySQL", "Хранение данных о товарах, корзинах и пользователях", $tags = "storage")
   
}
@enduml
```
# hl_mai_lab_01_vp
