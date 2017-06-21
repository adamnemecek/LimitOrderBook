# **Limit Order Book**
Build LOBs w. full snapshot

## Overview
 - Parse the raw binary data (format: Nasdaq TotalView- ITCH) and turn it into tick by tick information, taking care of different order/message types. Data files are **NOT** uploaded here. 
 - Build the limit order book (for one or multiple stocks) incrementally from the tick by tick trade ticker. The data structure is optimized for insertion/cancellation of limit orders, execution of market orders, etc. 
 - API supports queries on the Level 1 and Level 2 data as well as orders in queue of any price level.
