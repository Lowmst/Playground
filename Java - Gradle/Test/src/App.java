// Date 2023 / 09 / 17
// Author Liu Hongyuan
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.stage.Stage;
public class App extends Application {
    BorderPane pane = new BorderPane();
    Scene scene = new Scene(pane, 400, 300);
    Circle circle = new Circle(pane.getWidth() / 2, pane.getHeight() / 2, 50, Color.WHITE);
    @Override
    public void start(Stage stage) {
        circle.setStroke(Color.BLACK);
        pane.getChildren().add(circle);
        pane.setBottom(new ButtonBoard());
        stage.setTitle("E15_3");
        stage.setScene(scene);
        stage.show();
    }
    class ButtonBoard extends HBox {
        Button buttonLeft = new Button("Left");
        Button buttonRight = new Button("Right");
        Button buttonUp = new Button("Up");
        Button buttonDown = new Button("Down");
        double step = 50;
        public ButtonBoard() {
            buttonLeft.setOnAction(new CircleMove(circle, "Left", step));
            buttonRight.setOnAction(new CircleMove(circle, "Right", step));
            buttonUp.setOnAction(new CircleMove(circle, "Up", step));
            buttonDown.setOnAction(new CircleMove(circle, "Down", step));
            this.getChildren().addAll(buttonLeft, buttonRight, buttonUp, buttonDown);
            this.setAlignment(Pos.CENTER);
        }
    }
}
class CircleMove implements EventHandler<ActionEvent> {
    Circle circle;
    String direction;
    Pane pane;
    double step;
    public CircleMove(Circle circle, String direction, double step) {
        this.circle = circle;
        this.direction = direction;
        this.pane = (Pane)circle.getParent();
        this.step = step;
    }
    @Override
    public void handle(ActionEvent e) {
        switch (this.direction) {
            case "Left" :
                if ((circle.getCenterX() - circle.getRadius() - step) < 0) {
                    circle.setCenterX(circle.getRadius());
                } else {
                    circle.setCenterX(circle.getCenterX() - step);
                }
                break;
            case "Right" :
                if ((circle.getCenterX() + circle.getRadius() + step) > pane.getWidth()) {
                    circle.setCenterX(pane.getWidth() - circle.getRadius());
                } else {
                    circle.setCenterX(circle.getCenterX() + step);
                }
                break;
            case "Up" :
                if ((circle.getCenterY() - circle.getRadius() - step) < 0) {
                    circle.setCenterY(circle.getRadius());
                } else {
                    circle.setCenterY(circle.getCenterY() - step);
                }
                break;
            case "Down" :
                if ((circle.getCenterY() + circle.getRadius() + step) > pane.getHeight()) {
                    circle.setCenterY(pane.getHeight() - circle.getRadius());
                } else {
                    circle.setCenterY(circle.getCenterY() + step);
                }
                break;
        }
    }
}