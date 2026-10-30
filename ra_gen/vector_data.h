/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
        #ifdef __cplusplus
        extern "C" {
        #endif
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (17)
        #endif
        /* ISR prototypes */
        void sci_uart_rxi_isr(void);
        void sci_uart_txi_isr(void);
        void sci_uart_tei_isr(void);
        void sci_uart_eri_isr(void);
        void adc_scan_end_isr(void);

        /* Vector table allocations */
        #define VECTOR_NUMBER_SCI7_RXI ((IRQn_Type) 0) /* SCI7 RXI (Receive data full) */
        #define SCI7_RXI_IRQn          ((IRQn_Type) 0) /* SCI7 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI7_TXI ((IRQn_Type) 1) /* SCI7 TXI (Transmit data empty) */
        #define SCI7_TXI_IRQn          ((IRQn_Type) 1) /* SCI7 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI7_TEI ((IRQn_Type) 2) /* SCI7 TEI (Transmit end) */
        #define SCI7_TEI_IRQn          ((IRQn_Type) 2) /* SCI7 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI7_ERI ((IRQn_Type) 3) /* SCI7 ERI (Receive error) */
        #define SCI7_ERI_IRQn          ((IRQn_Type) 3) /* SCI7 ERI (Receive error) */
        #define VECTOR_NUMBER_ADC0_SCAN_END ((IRQn_Type) 4) /* ADC0 SCAN END (End of A/D scanning operation) */
        #define ADC0_SCAN_END_IRQn          ((IRQn_Type) 4) /* ADC0 SCAN END (End of A/D scanning operation) */
        #define VECTOR_NUMBER_SCI2_RXI ((IRQn_Type) 5) /* SCI2 RXI (Receive data full) */
        #define SCI2_RXI_IRQn          ((IRQn_Type) 5) /* SCI2 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI2_TXI ((IRQn_Type) 6) /* SCI2 TXI (Transmit data empty) */
        #define SCI2_TXI_IRQn          ((IRQn_Type) 6) /* SCI2 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI2_TEI ((IRQn_Type) 7) /* SCI2 TEI (Transmit end) */
        #define SCI2_TEI_IRQn          ((IRQn_Type) 7) /* SCI2 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI2_ERI ((IRQn_Type) 8) /* SCI2 ERI (Receive error) */
        #define SCI2_ERI_IRQn          ((IRQn_Type) 8) /* SCI2 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI4_RXI ((IRQn_Type) 9) /* SCI4 RXI (Receive data full) */
        #define SCI4_RXI_IRQn          ((IRQn_Type) 9) /* SCI4 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI4_TXI ((IRQn_Type) 10) /* SCI4 TXI (Transmit data empty) */
        #define SCI4_TXI_IRQn          ((IRQn_Type) 10) /* SCI4 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI4_TEI ((IRQn_Type) 11) /* SCI4 TEI (Transmit end) */
        #define SCI4_TEI_IRQn          ((IRQn_Type) 11) /* SCI4 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI4_ERI ((IRQn_Type) 12) /* SCI4 ERI (Receive error) */
        #define SCI4_ERI_IRQn          ((IRQn_Type) 12) /* SCI4 ERI (Receive error) */
        #define VECTOR_NUMBER_SCI9_RXI ((IRQn_Type) 13) /* SCI9 RXI (Receive data full) */
        #define SCI9_RXI_IRQn          ((IRQn_Type) 13) /* SCI9 RXI (Receive data full) */
        #define VECTOR_NUMBER_SCI9_TXI ((IRQn_Type) 14) /* SCI9 TXI (Transmit data empty) */
        #define SCI9_TXI_IRQn          ((IRQn_Type) 14) /* SCI9 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI9_TEI ((IRQn_Type) 15) /* SCI9 TEI (Transmit end) */
        #define SCI9_TEI_IRQn          ((IRQn_Type) 15) /* SCI9 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI9_ERI ((IRQn_Type) 16) /* SCI9 ERI (Receive error) */
        #define SCI9_ERI_IRQn          ((IRQn_Type) 16) /* SCI9 ERI (Receive error) */
        /* The number of entries required for the ICU vector table. */
        #define BSP_ICU_VECTOR_NUM_ENTRIES (17)

        #ifdef __cplusplus
        }
        #endif
        #endif /* VECTOR_DATA_H */