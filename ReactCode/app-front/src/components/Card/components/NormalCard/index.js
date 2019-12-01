import React from 'react'

import './index.less'

const NormalCard = ({type, data, bool}) => {
 
  switch (type) {
    case "fall":
    case "coGas":
      break
    default:
      break
  }
  return (
    <div>
      {(bool) ? ((data==='')? "Nenhum dado encontrado" : (data) ? "Algo de errado aconteceu!" : "Está tudo bem") : data }
   </div>
  )
}

export default NormalCard
