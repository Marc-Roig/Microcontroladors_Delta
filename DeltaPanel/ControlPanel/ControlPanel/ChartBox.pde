import controlP5.*;

public class ChartBox {

    ControlP5 theControlP5;

    ControlGroup BoxGroup;
    String BoxName;

    Chart attachedChart;

    int font_size;

    float graph_ypos; 
    float graph_xpos; 
  
    float graph_width; 
    float graph_height;

    int box_width;
    int height_per_item = 28;
    int space_between_item_and_value = 40;
    int box_height;

    ArrayList<String> items = new ArrayList<String>();
    ArrayList<String> items_values = new ArrayList<String>();
    int num_of_items = 0;

    public ChartBox(ControlP5 theControlP5_, Chart chart_, String name) {

        theControlP5 = theControlP5_;

        attachedChart = chart_;
        BoxName = name;

        font_size = 14;
        box_width = 120;
    
        init();

    }

    public ChartBox updateParameters() {

        graph_xpos = attachedChart.getPosition()[0];
        graph_ypos = attachedChart.getPosition()[1];

        graph_width = attachedChart.getWidth();
        graph_height = attachedChart.getHeight();

        return this;

    }

    public void init() {

        updateParameters();

        // create a group to store the servo values
        BoxGroup = theControlP5.addGroup(" " + BoxName + "  Values", (int)(graph_xpos + graph_width - box_width), (int)(graph_ypos + 10), box_width);
        BoxGroup.setBackgroundHeight(height_per_item * num_of_items);
        BoxGroup.setBackgroundColor(color(134, 124, 134, 100));

        BoxGroup.setFont(createFont("", 11));
        
        BoxGroup.close();

    }

    public ChartBox addItem(String item_, String value) {

        items.add(item_);
        items_values.add(value);

        items.add(item_);
        items_values.add(value);

        Textlabel duty_cycle_s1 = theControlP5.addTextlabel(BoxName + item_, item_, 10, 4 + height_per_item * num_of_items);
        duty_cycle_s1.setFont(createFont("", font_size));
        duty_cycle_s1.moveTo(BoxGroup);

        Textlabel duty_cycle_val1 = theControlP5.addTextlabel(BoxName + item_ + "_val", value, 10 + space_between_item_and_value, 4 + height_per_item * num_of_items );
        duty_cycle_val1.setFont(createFont("", font_size));
        duty_cycle_val1.moveTo(BoxGroup);

        num_of_items++;
        box_height = height_per_item * num_of_items + 4;
        BoxGroup.setBackgroundHeight(box_height);

        return this;

    }

    public ChartBox updateItemsPositions() {

        return this;

    }

    public ChartBox assignItemValue(String item_, String value_) {
        
        int index = items.indexOf(item_);
        // println(index);
        if (index == -1) return this; //invalid item

        items_values.set(index, value_);

        Textlabel t = (Textlabel)theControlP5.getController(BoxName + item_ + "_val");
        t.setText(value_);

        return this;

    }

    public ChartBox setBarHeight(int barHeight) {

        BoxGroup.setBarHeight(barHeight);
        BoxGroup.setPosition((int)(graph_xpos + graph_width - box_width), (int)(graph_ypos + barHeight));

        return this;

    }

    public ChartBox setFontSize(int size) {

        font_size = size;

        return this;

    }


}