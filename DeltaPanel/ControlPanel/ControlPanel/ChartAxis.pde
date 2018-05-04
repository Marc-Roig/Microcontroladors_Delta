import controlP5.*;

public class ChartAxis {
   
    Chart attachedChart;

    float graph_ypos; 
    float graph_xpos; 
  
    float graph_width; 
    float graph_height;
  
    int num_of_x_lines;
    float perc_per_x_line;
    int num_x_vals_to_show;
  
    int num_of_y_lines;
    float perc_per_y_line;
  
    float total_time; //in seconds 

    boolean xaxis_enabled = true;
    boolean xaxis_labels_enabled = true;
    boolean yaxis_enabled = false;
    boolean yaxis_labels_enabled = true;

    boolean contourn_enabled = false;

    boolean hor_guides_enabled = false; 

    // public ChartAxis(Chart chart_) {
    public ChartAxis(Chart chart_) {

        attachedChart = chart_;

        updateParameters();

        num_of_x_lines = 20;
        perc_per_x_line = 1/float(num_of_x_lines);
        num_of_y_lines = 6;
        perc_per_y_line = 1/float(num_of_y_lines);

        num_x_vals_to_show = 4;

        total_time = 30; 

    }

    public ChartAxis updateParameters() {

        graph_xpos = attachedChart.getPosition()[0];
        graph_ypos = attachedChart.getPosition()[1];

        graph_width = attachedChart.getWidth();
        graph_height = attachedChart.getHeight();

        return this;
        
    }

    public void draw() {

        strokeWeight(1);
        stroke(100);
        textSize(9);
        fill(0);

        if (xaxis_enabled) {

            textAlign(CENTER);

            for (int i = 0; i < num_of_x_lines + 1; i++) {
                line(graph_xpos + graph_width * (i*perc_per_x_line), graph_ypos, graph_xpos + graph_width * (i*perc_per_x_line), graph_ypos - 10);
            }

            if (xaxis_labels_enabled) {

                for (int i = 0; i < num_of_x_lines + 1; i+=num_of_x_lines/num_x_vals_to_show) {
                    text(total_time*i*perc_per_x_line, graph_xpos + graph_width * (i*perc_per_x_line), graph_ypos - 12);
                }

            }

        }

        if (yaxis_enabled) {

            textAlign(RIGHT);

            for (int i = 0; i < num_of_y_lines + 1; i++) {
                line(graph_xpos, graph_ypos + graph_height * (i*perc_per_y_line), graph_xpos-10, graph_ypos + graph_height * (i*perc_per_y_line));  
                if (yaxis_labels_enabled) text((int)ceil(180*(1 - (i*perc_per_y_line))), graph_xpos - 15, graph_ypos + graph_height * (i*perc_per_y_line) + 4);
            }
            
        }

        if (hor_guides_enabled) {

            strokeWeight(1);
            stroke(255, 255, 255, 20);

            for (float i = 1; i < num_of_y_lines; i++) {
                line(graph_xpos, graph_ypos + graph_height * (i*perc_per_y_line), graph_xpos + graph_width, graph_ypos + graph_height * (i*perc_per_y_line));
            }

        }

        if (contourn_enabled) {

            strokeWeight(4);
            stroke(155);

            line(graph_xpos - 2, graph_ypos, graph_xpos - 2, graph_ypos + graph_height); 
            line(graph_xpos, graph_ypos - 2, graph_xpos + graph_width, graph_ypos - 2);
            line(graph_xpos, graph_ypos + 1 + graph_height, graph_xpos + graph_width, graph_ypos + graph_height + 1); 
            line(graph_xpos + graph_width + 1, graph_ypos, graph_xpos  + graph_width + 1, graph_ypos + graph_height); 

        }

    }

    public ChartAxis setNumOfIntervals(int x_num, int y_num) {

        num_of_x_lines = x_num;
        perc_per_x_line = 1/float(num_of_x_lines);

        num_of_y_lines = y_num;
        perc_per_y_line = 1/float(num_of_y_lines);

        return this;

    }

    public ChartAxis setNumOfIntervals(int x_num, int y_num, int x_labels_to_show) {

        num_of_x_lines = x_num;
        perc_per_x_line = 1/float(num_of_x_lines);

        num_x_vals_to_show = x_labels_to_show;

        num_of_y_lines = y_num;
        perc_per_y_line = 1/float(num_of_y_lines);

        return this;

    }

    public ChartAxis setNumOfXIntervals(int num) {

        num_of_x_lines = num;
        perc_per_x_line = 1/float(num_of_x_lines);

        return this;

    }

    public ChartAxis setNumOfXIntervals(int num, int amount_to_show) {

        num_of_x_lines = num;
        perc_per_x_line = 1/float(num_of_x_lines);
        num_x_vals_to_show = amount_to_show;

        return this;

    }

    public ChartAxis setNumOfYIntervals(int num) {

        num_of_y_lines = num;
        perc_per_y_line = 1/float(num_of_y_lines);
    
        return this;
    }

    
    public ChartAxis setTotalTime(int total_time_) {
      
        total_time = total_time_;
        
        return this;  
    }

    public ChartAxis enableHorGuides() {

        hor_guides_enabled = true;

        return this;

    }

    public ChartAxis disableHorGuides() {

        hor_guides_enabled = false;

        return this;

    }
    
    public ChartAxis enableXlabels() {

        xaxis_labels_enabled = true;
    
        return this;
    }
    
    public ChartAxis disableXlabels() {

        xaxis_labels_enabled = false;
    
        return this;
    }
    
    public ChartAxis enableYlabels() {

        yaxis_labels_enabled = true;
    
        return this;
    }
    
    public ChartAxis disableYlabels() {

        yaxis_labels_enabled = false;
    
        return this;
    }

    public ChartAxis enableContourn() {

        contourn_enabled = true;
    
        return this;
    }

    public ChartAxis disableContourn() {

        contourn_enabled = false;
    
        return this;
    }
    
    public ChartAxis enableXAxis() {
      
      xaxis_enabled = true;
        
        return this;  
    }
    
    public ChartAxis disablexAxis() {
    
      xaxis_enabled = false;
        
        return this;  
    }
    
    public ChartAxis enableYAxis() {
      
      yaxis_enabled = true;
        
        return this;  
    }
    
    public ChartAxis disableYAxis() {
    
      yaxis_enabled = false;
        
        return this;  

    }
    
    
    
  
}