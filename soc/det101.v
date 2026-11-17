`timescale 1ns/1ps
`default_nettype none

module det101 (
    input      clk,
    input      rst_n,
    input      x,
    output     z
);
    localparam S0 = 2'd0, S1 = 2'd1, S2 = 2'd2, S3 = 2'd3;
    reg [1:0] state, next_state;

    assign z = (state == S3);                        // ③ 输出只看状态

    always @(posedge clk or negedge rst_n)           // ①
        if (!rst_n) state <= S0;
        else        state <= next_state;

    always @(*)                                      // ②
        case (state)
            S0: next_state = x ? S1 : S0;
            S1: next_state = x ? S1 : S2;
            S2: next_state = x ? S3 : S0;
            S3: next_state = x ? S1 : S2;
        endcase
endmodule
