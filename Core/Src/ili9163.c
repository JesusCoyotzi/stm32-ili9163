#include "ili9163.h"


void writeSpiCmd(uint8_t cmd )
{
  //Send command address via SPI
  // Needs to use writeSpiData in case command requires data
  HAL_GPIO_WritePin(disp.cs_port,disp.cs_pin,0); 
  HAL_GPIO_WritePin(disp.dc_port,disp.dc_pin,0); 
   
  // Last parameter is timeout
  HAL_SPI_Transmit(&disp.spiHand,&cmd,1,10);

  HAL_GPIO_WritePin(disp.cs_port,disp.cs_pin,1); 
}

void writeSpiData(uint8_t data)
{
  //Send data via SPI
  HAL_GPIO_WritePin(disp.cs_port,disp.cs_pin,0); 
  HAL_GPIO_WritePin(disp.dc_port,disp.dc_pin,1); 
   
  // Last parameter is timeout
  HAL_SPI_Transmit(&disp.spiHand,&data,1,10);

  HAL_GPIO_WritePin(disp.cs_port,disp.cs_pin,1); 
}


void resetDisp(){
  HAL_GPIO_WritePin(disp.rst_port,disp.rst_pin,0);
  HAL_Delay(15);

  HAL_GPIO_WritePin(disp.rst_port,disp.rst_pin,1);
  HAL_Delay(15);
}

void renderBuf(uint8_t* buff)
{
  writeSpiDataBuffer(buff, BUFSIZE);
}

void writeSpiDataBuffer(uint8_t * buffer, uint16_t len)
{
  //Send data via SPI
  HAL_GPIO_WritePin(disp.cs_port,disp.cs_pin,0); 
  HAL_GPIO_WritePin(disp.dc_port,disp.dc_pin,1); 
   
  // Last parameter is timeout
  HAL_SPI_Transmit(&disp.spiHand,buffer,len,1000);

  HAL_GPIO_WritePin(disp.cs_port,disp.cs_pin,1); 
  return;
}

void initDisp()
{
  // RST and Chip select are active low
  HAL_GPIO_WritePin(disp.rst_port,disp.rst_pin,1);
  HAL_GPIO_WritePin(disp.cs_port,disp.cs_pin,1); 

  resetDisp(disp);  
    
  //writeSpiCmd(ILI9163_CMD_SOFT_RESET);
  // Wake up 
  writeSpiCmd(ILI9163_CMD_EXIT_SLEEP_MODE);
  HAL_Delay(20);

  // Set gamma curve
  writeSpiCmd(ILI9163_CMD_SET_GAMMA_CURVE);
  writeSpiData(0x04);


  // Set framerate on normal mode
  writeSpiCmd(ILI9163_CMD_FRAME_RATE_NORMAL);
  writeSpiData(0x0C);
  writeSpiData(0x14);

  //Power values
  writeSpiCmd(ILI9163_CMD_POWER_CONTROL1);
  writeSpiData(0x0C); //VRH
  writeSpiData(0x05); //VC

  writeSpiCmd(ILI9163_CMD_POWER_CONTROL2);
  writeSpiData(0x02); //VRH
  
  writeSpiCmd(ILI9163_CMD_POWER_CONTROL3);
  writeSpiData(0x02); //VRH

  writeSpiCmd(ILI9163_CMD_VCOM_CONTROL1);
  writeSpiData(0x20); //VMH
  writeSpiData(0x55); //VML

  writeSpiCmd(ILI9163_CMD_VCOM_OFFSET_CONTROL);
  writeSpiData(0x40); //VMH


  // Setting drawing area
  writeSpiCmd(ILI9163_CMD_SET_COLUMN_ADDRESS);
  writeSpiData(0x00); //XSH
  writeSpiData(0x00); //XSL
  writeSpiData(0x00); //XEH
  writeSpiData(ILI9163_HEIGHT-1); //XEL

  writeSpiCmd(ILI9163_CMD_SET_PAGE_ADDRESS);
  writeSpiData(0x00); //XSH
  writeSpiData(0x00); //XSL
  writeSpiData(0x00); //XEH
  writeSpiData(ILI9163_WIDTH-1); //XEL
  
  //set pixel format
  writeSpiCmd(ILI9163_CMD_SET_PIXEL_FORMAT);
  writeSpiData(0x06); //6-6-6

  // Set drawing direction
  writeSpiCmd(ILI9163_CMD_SET_ADDRESS_MODE);
  writeSpiData(0x40 | 0x20 | 0x08);

  // Driber direction
  writeSpiCmd(ILI9163_CMD_SOURCE_DRIVER_DIRECTION);
  writeSpiData(0x00);
  

  uint8_t pgam[16] =  { 0x36, 0x29, 0x12, 0x22, 0x1C, 0x15, 0x42, 0xB7, 0x2F, 0x13, 0x12, 0x0A, 0x11, 0x0B, 0x06 };
  writeSpiCmd(ILI9163_CMD_POSITIVE_GAMMA_CORRECT);
  writeSpiDataBuffer(pgam,16);

  uint8_t ngam[16] = { 0x09, 0x16, 0x2D, 0x0D, 0x13, 0x15, 0x40, 0x48, 0x53, 0x0C, 0x1D, 0x25, 0x2E, 0x34, 0x39 };
  writeSpiCmd(ILI9163_CMD_NEGATIVE_GAMMA_CORRECT);
  writeSpiDataBuffer(ngam,16);


  //writeSpiCmd(ILI9163_CMD_ENTER_NORMAL_MODE);
  writeSpiCmd(ILI9163_CMD_SET_DISPLAY_ON);
  writeSpiCmd(ILI9163_CMD_WRITE_MEMORY_START);
  return;
}


